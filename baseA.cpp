#include<baseA.h>

void image(void* odt, Area* iarea) {
	Image* i = (Image*)iarea;
	int* d = (int*)odt;

	if (!d[0]) {
		i->setNO(0);
	}
	else {
		i->setNO(d[1] + 1);
	}
}

void list(void* odt, Area* iarea) {
	List* l = (List*)iarea;
	int* d = (int*)odt;

	l->setChoose(d[1]);
}

void button(void* odt, Area* iarea) {
	Button* b = (Button*)iarea;
	int* d = (int*)odt;

	b->setData(d[0]);
}