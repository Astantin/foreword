#include<easyx.h>
#include <iostream>

struct Order {
	int pos;
	int num;
	Order* next;
};


class Area;
typedef class MouseSqare {
private:
	Area* iarea;
	int x;
	int y;
	int dx;
	int dy;
	bool islive;
	bool iA;//µã»÷
	bool iB;//ÐüÍ£
	MouseSqare* link;
public:
	MouseSqare(
		Area* iarea,
		int x,
		int y,
		int dx,
		int dy,
		bool islive,
		bool iA,
		bool iB,
		MouseSqare* link);
	void setlive(bool);
	void setlink(MouseSqare*);
	MouseSqare* next();
	bool isReady(int x, int y);
	Order* ord;
	void (*behavior)(void*);
	void run(void*);
}MSquare;


class Manger {
private:
	void* odt;
	MSquare* hdat;
	MSquare* ldat;
public:
	Manger(void*);
	// #new fuc
	MSquare* add(Area* iarea, bool iA, bool iB, Order* ord, bool islive, int x, int y, int dx, int dy);
	MSquare* addX(Area* a, int start);
	void pop();
	void setlive();
	void run(int x,int y);
};


class Area {
protected:
	IMAGE* images;
	MouseSqare* sqares;
	Area* links;
	int x;
	int y;
	int dx;
	int dy;
	void nextReset(void*);
public:
	Area(int x, int y, int dx, int dy);
	void (*script)(void*, Area*);
	virtual void set(IMAGE* imgs, MouseSqare* ms, Area* lks) = 0;
	virtual void reset(void*) = 0;
	virtual MSquare* setMS(int start, Manger* mg) = 0;
	void hide();
	// #new fuc
	int* getXY();
};


class Button :public Area {
private:
	bool isOff;
public:
	Button(int x, int y, int dx, int dy);
	void set(IMAGE* imgs, MouseSqare* ms, Area* lks);
	void reset(void*);
	MSquare* setMS(int start, Manger* mg);
	void setData(bool isoff);
};


class List :public Area {
private:
	bool isVertical;
	bool isMid; 
	int msize;
	int size;
	int idx;
	int idy;
	int space;
	
	int* listData;//ËØ²Ä±àºÅ´Ó1¿ªÊ¼
	int choose;
public:
	List(int x, int y, int dx, int dy, bool v, bool m, int msize, int idx, int idy, int space, int size = 0);
	void set(IMAGE* imgs, MouseSqare* ms, Area* lks);
	void reset(void*);
	int* getData();
	void setData(int*);
	void setChoose(int);
	//#new fuc
	MSquare* setMS(int start, Manger* mg);
};


typedef class PageList :public Area {
private:

public:
	void set(IMAGE* imgs, MouseSqare* ms, Area* lks);
	void reset(void*);
	MSquare* setMS(int start, Manger* mg);
}PList;


class Image :public Area {
private:
	int img_NO;// ËØ²Ä±àºÅ´Ó0¿ªÊ¼
public:
	Image(int x, int y, int dx, int dy);
	void set(IMAGE* imgs, MouseSqare* ms, Area* lks);
	void reset(void*);
	MSquare* setMS(int start, Manger* mg);
	void setNO(int);
};


typedef class UnlimitedImage :public Area {
private:

public:
	void set(IMAGE* imgs, MouseSqare* ms, Area* lks);
	void reset(void*);
	MSquare* setMS(int start, Manger* mg);
}ULImage;
