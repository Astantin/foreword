#include <easyx.h>
#include <baseA.h>

int* odata = new int[64];

Manger* mg = new Manger(odata);

int main(){
	IMAGE bk;
	loadimage(&bk, L"images\\bk.png", 1000, 800);

	IMAGE* imI = new IMAGE[10], * imL = new IMAGE[10], * imB = new IMAGE[10];

	loadimage(&imI[1], L"images\\imred.png", 318, 210); loadimage(&imI[2], L"images\\imyellow.png", 318, 210);
	loadimage(&imI[3], L"images\\imblue.png", 318, 210); loadimage(&imI[4], L"images\\imgreen.png", 318, 210);
	
	loadimage(&imL[1], L"images\\lstbk.png", 600, 180); loadimage(&imL[2], L"images\\lstyellow0.png", 100, 130);
	loadimage(&imL[3], L"images\\lstyellow1.png", 100, 130); loadimage(&imL[4], L"images\\lstblue0.png", 100, 130);
	loadimage(&imL[5], L"images\\lstblue1.png", 100, 130); loadimage(&imL[6], L"images\\lstgreen0.png", 100, 130);
	loadimage(&imL[7], L"images\\lstgreen1.png", 100, 130);

	loadimage(&imB[1], L"images\\button1.png", 220, 85); loadimage(&imB[2], L"images\\button0.png", 220, 85);

	Image* a = new Image(272, 63, 318, 210); 
	List* b = new List(114, 385, 600, 180, false, false, 3, 100, 130, 20, 3);
	int* bb = b->getData(); bb[0] = 1; bb[1] = 2; bb[2] = 3; 
	Button* c = new Button(686, 121, 220, 85);

	initgraph(1000, 800); putimage(0, 0, &bk);

	odata[0] = 1; odata[1] = -1; odata[2] = 0;
	a->set(imI, mg->addX(a, 2), NULL); a->script = image;
	b->set(imL, mg->addX(b, 1), a); b->script = list;
	c->set(imB, mg->addX(c, 0), a); c->script = button;
	
	ExMessage msg;
	while (1) {
		msg = getmessage();
		if (msg.lbutton) {
			mg->run(msg.x, msg.y);
		}
	}
}
