#include<baseB.h>

/* class MouseSqare */
MouseSqare::MouseSqare(Area* iarea, int x, int y, int dx, int dy, bool islive, bool iA, bool iB, MouseSqare* link) {
	this->iarea = iarea;
	this->x = x;
	this->y = y;
	this->dx = dx;
	this->dy = dy;
	this->islive = islive;
	this->iA = iA;
	this->iB = iB;
	this->link = link;
	ord = NULL;
	behavior = NULL;
}

bool MouseSqare::isReady(int px, int py) {
	if (islive) {
		if (px > x && px < x + dx && py > y && py < y + dy) {
			return true;
		}
	}
	return false;
}

void MouseSqare::setlive(bool live) {
	islive = live;
}

void MouseSqare::setlink(MouseSqare* next) {
	link = next;
}

MouseSqare* MouseSqare::next() {
	return link;
}

void MouseSqare::run(void* odt) {
	int* i = (int*)odt;
	if (behavior) { behavior(odt); }
	Order* o = ord;
	while (o) {
		i[o->pos] = o->num;
		o = o->next;
	}
	iarea->reset(odt);
}


/* class Manger */
Manger::Manger(void* odt){
	hdat = NULL;
	ldat = NULL;
	this->odt = odt;
}

MSquare* Manger::add(Area* iarea, bool iA, bool iB, Order* ord, bool islive = true,int x = 0, int y = 0, int dx = 0, int dy = 0){
	if (dx == 0 || dy == 0) {
		int* xy = iarea->getXY();
		x = xy[0]; y = xy[1];
		dx = xy[2]; dy = xy[3];
		delete xy;
	}
	MouseSqare* m = new MouseSqare(iarea, x, y, dx, dy, true, iA, iB, NULL);
	m->ord = ord;
	if (!ldat) {
		hdat = ldat = m;
	}
	else {
		ldat->setlink(m);
		ldat = m;
	}
	return m;
}

MSquare* Manger::addX(Area* a, int start) {
	return a->setMS(start, this);
}

void Manger::run(int x, int y){
	MouseSqare* m = hdat;
	while (m) {
		if (m->isReady(x, y)) {
			m->run(odt);
		}
		m = m->next();
	}
}


/* class Area */
Area::Area(int x, int y, int dx, int dy) {
	script = NULL;
	links = NULL;
	images = NULL;
	sqares = NULL;
	this->x = x; this->y = y;
	this->dx = dx; this->dy = dy;
}

void Area::hide() {
	putimage(x, y, &images[0]);
}

int* Area::getXY() {
	int* xy = new int[4];
	xy[0] = x; xy[1] = y;
	xy[2] = dx; xy[3] = dy;
	return xy;
}

void Area::nextReset(void* odt) {
	Area* na = links;
	while (na) {
		na->reset(odt);
		na = na->links;
	}
}


/* class Button : public Area */
void MSButton(void* odt) {
	int* d = (int*)odt;
	if (d[0]) {
		d[0] = 0;
	}
	else {
		d[0] = 1;
	}
}

Button::Button(int x, int y, int dx, int dy) :Area(x, y, dx, dy) {
	isOff = true;
}

void Button::set(IMAGE* imgs, MouseSqare* ms, Area* lks) {  //自身的工厂方法，组装出完整的Area
	images = imgs; sqares = ms; links = lks;
	getimage(&images[0], x, y, dx, dy);   //背景
	if (!isOff) {
		putimage(x, y, &images[1]);
	}
	else {
		putimage(x, y,&images[2]);
	}
}

void Button::reset(void* odt) {
	if (script) { script(odt, this); }
	if (!isOff) {
		putimage(x, y, &images[1]);
	}
	else {
		putimage(x, y, &images[2]);
	}
	nextReset(odt);
}

MSquare* Button::setMS(int start, Manger* mg) {
	MSquare* m = mg->add(this, true, false, NULL);
	m->behavior = MSButton;
	return m;
}

void Button::setData(bool isoff) {
	isOff = isoff;
}


/* class List : public Area */
List::List(int x, int y, int dx, int dy, bool v, bool m, int msize, int idx, int idy, int space,int size) :Area(x, y, dx, dy) {
	isVertical = v; isMid = m; 
	this->msize = msize; this->idx = idx; this->idy = idy; this->space = space; this->size = size;
	listData = new int[size] {}; choose = -1;
}

void List::set(IMAGE* imgs, MouseSqare* ms, Area* lks) {
	images = imgs; sqares = ms; links = lks;
	getimage(&images[0], x, y, dx, dy);   
	putimage(x, y, &images[1]);
	if (isVertical) {
		for (int i = 0; i < msize; i++) {
			if (listData[i]) {
				if (!isMid) {
					putimage(x + (dx - idx) / 2, y + (i + 1) * space + i * idy, &images[listData[i] * 2]);
				}
				else {
					putimage(x + (dx - idx) / 2, i * (space + idy) + y + (dy - (size * idy + (size - 1) * space)) / 2, &images[listData[i] * 2]);
				}
			}
			else {
				break;
			}
		}
	}
	else {
		for (int i = 0; i < msize; i++) {
			if (listData[i]) {
				if (!isMid) {
					putimage(x + (i + 1) * space + i * idx, y + (dy - idy) / 2, &images[listData[i] * 2]);
				}
				else {
					putimage(i * (space + idx) + x + (dx - (size * idx + (size - 1) * space)) / 2, y + (dy - idy) / 2, &images[listData[i] * 2]);
				}
			}
			else {
				break;
			}
		}
	}
}

MSquare* List::setMS(int start, Manger* mg) {
	MSquare* m, * n = NULL;
	if (isVertical) {
		for (int i = 0; i < msize; i++) {
			if (i < size) {
				if (!isMid) {
					Order* o = new Order{ start,i ,NULL };
					m = mg->add(this, true, false, o, true, x + (dx - idx) / 2, y + (i + 1) * space + i * idy, idx, idy);
				}
				else {
					Order* o = new Order{ start,i ,NULL };
					m = mg->add(this, true, false, o, true, x + (dx - idx) / 2, i * (space + idy) + y + (dy - (size * idy + (size - 1) * space)) / 2, idx, idy);
				}
			}
			else {
				if (!isMid) {
					Order* o = new Order{ start,i ,NULL };
					m = mg->add(this, true, false, o, false, x + (dx - idx) / 2, y + (i + 1) * space + i * idy, idx, idy);
				}
				else {
					Order* o = new Order{ start,i ,NULL };
					m = mg->add(this, true, false, o, false, x + (dx - idx) / 2, i * (space + idy) + y + (dy - (size * idy + (size - 1) * space)) / 2, idx, idy);
				}
			}
			if (i == 0) { n = m; }
		}
	}
	else {
		for (int i = 0; i < msize; i++) {
			if (i < size) {
				if (!isMid) {
					Order* o = new Order{ start,i ,NULL };
					m = mg->add(this, true, false, o, true, x + (i + 1) * space + i * idx, y + (dy - idy) / 2, idx, idy);
				}
				else {
					Order* o = new Order{ start,i ,NULL };
					m = mg->add(this, true, false, o, true, i * (space + idx) + x + (dx - (size * idx + (size - 1) * space)) / 2, y + (dy - idy) / 2, idx, idy);
				}
			}
			else {
				if (!isMid) {
					Order* o = new Order{ start,i ,NULL };
					m = mg->add(this, false, false, o, true, x + (i + 1) * space + i * idx, y + (dy - idy) / 2, idx, idy);
				}
				else {
					Order* o = new Order{ start,i ,NULL };
					m = mg->add(this, false, false, o, true, i * (space + idx) + x + (dx - (size * idx + (size - 1) * space)) / 2, y + (dy - idy) / 2, idx, idy);
				}
			}
			if (i == 0) { n = m; }
		}
	}
	return n;
}

void List::reset(void* odt) {
	if (script) { script(odt, this); }
	putimage(x, y, &images[1]);
	if (isVertical) {
		for (int i = 0; i < msize; i++) {
			if (listData[i]) {
				if (!isMid) {
					putimage(x + (dx - idx) / 2, y + (i + 1) * space + i * idy, &images[listData[i] * 2]);
				}
				else {
					putimage(x + (dx - idx) / 2, i * (space + idy) + y + (dy - (size * idy + (size - 1) * space)) / 2, &images[listData[i] * 2]);
				}
			}
			else {
				break;
			}
		}
		if (!isMid) {
			putimage(x + (dx - idx) / 2, y + (choose + 1) * space + choose * idy, &images[listData[choose] * 2 + 1]);
		}
		else {
			putimage(x + (dx - idx) / 2, choose * (space + idy) + y + (dy - (size * idy + (size - 1) * space)) / 2, &images[listData[choose] * 2 + 1]);
		}
	}
	else {
		for (int i = 0; i < msize; i++) {
			if (listData[i]) {
				if (!isMid) {
					putimage(x + (i + 1) * space + i * idx, y + (dy - idy) / 2, &images[listData[i] * 2]);
				}
				else {
					putimage(i * (space + idx) + x + (dx - (size * idx + (size - 1) * space)) / 2, y + (dy - idy) / 2, &images[listData[i] * 2]);
				}
			}
			else {
				break;
			}
		}
		if (!isMid) {
			putimage(x + (choose + 1) * space + choose * idx, y + (dy - idy) / 2, &images[listData[choose] * 2 + 1]);
		}
		else {
			putimage(choose * (space + idx) + x + (dx - (size * idx + (size - 1) * space)) / 2, y + (dy - idy) / 2, &images[listData[choose] * 2 + 1]);
		}
	}
	nextReset(odt);
}

int* List::getData() {
	return listData;
}

void List::setData(int* data) {
	listData = data;
}

void List::setChoose(int c) {
	choose = c;
}


/* class PageList : public Area */
void PageList::set(IMAGE* imgs, MouseSqare* ms, Area* lks) {

}

void PageList::reset(void* odt) {
	
}

MSquare* PageList::setMS(int start, Manger* mg) {
	return NULL;
}


/* class Image : public Area */
Image::Image(int x, int y, int dx, int dy) :Area(x, y, dx, dy) {
	img_NO = 0;
}

void Image::set(IMAGE* imgs, MouseSqare* ms, Area* lks) {
	images = imgs; sqares = ms; links = lks;
	getimage(&images[0], x, y, dx, dy);
	putimage(x, y, &images[img_NO + 1]);
}

void Image::reset(void* odt) {
	if (script) { script(odt, this); }
	putimage(x, y, &images[img_NO + 1]);
	nextReset(odt);
}

MSquare* Image::setMS(int start, Manger* mg) {
	return NULL;
}

void Image::setNO(int n) {
	img_NO = n;
}


/* class UnlimitedImage : public Area */
void UnlimitedImage::set(IMAGE* imgs, MouseSqare* ms, Area* lks) {

}

void UnlimitedImage::reset(void*) {

}

MSquare* UnlimitedImage::setMS(int start, Manger* mg) {
	return NULL;
}
