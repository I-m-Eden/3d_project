//头文件
#include "3dpaint.h"
#include <time.h>
#include <string>
#include <iostream>
using namespace std;
#define ref(i,x,y)for(int i=(x);i<=(y);++i)
string constr(int s) {
	string res = "";
	if (s < 0)res = "-" + constr(-s); else
		if (s < 10)res = res + char(s + 48); else
			res = constr(s / 10), res = res + char(s % 10 + 48);
	return res;
}

/*
void painttest() {
	int r = 1;
	while (1) {
		peekmsg();
		if (iskeydown(VK_DOWN) && r > 1)r--;
		if (iskeydown(VK_UP))r++;
		if (iskeydown(VK_ESCAPE))break;
		clearscreen(rgb(0, 255, 0));
		setd(0, 4, rgb(255, 0, 0));
		setf(rgb(0, 0, 255));
		fbar(400 - r, 300 - r, 400 + r, 300 + r);
		setf(rgb(0, 255, 255));
		pcircle(400, 300, r);
		pline(400 - r, 300 - r, 400 + r, 300 + r);
		setd(0, 6, rgb(255, 255, 0));
		dellipse(400 - r, 300 - r / 2, 400 + r, 300 + r / 2);
		setd(0, 3, rgb(255, 0, 255));
		setf(5, rgb(122, 0, 122));
		POINT pt[] = { { 400 - r,300 - r },{ 400,300 - r / 2 },{ 400 + r,300 - r },{ 400 + r / 2,300 },
		{ 400 + r,300 + r },{ 400,300 + r / 2 },{ 400 - r,300 + r },{ 400 - r / 2,300 } };
		ppolygon(pt, 8);

		setf(0, rgb(122, 255, 0));
		setd(0, 2, rgb(255, 122, 0));
		pchord(400 - r, 300 - r, 400 + r, 300 + r, 400 + r, 300 - r / 2, 400 - r, 300 - r / 2);
		pchord(400 - r, 300 - r, 400 + r, 300 + r, 400 - r, 300 + r / 2, 400 + r, 300 + r / 2);

		setd(0, 10, rgb(0, r / 2 % 256, r / 2 % 256));
		parc(400 - r, 300 - r, 400 + r, 300 + r, 800, 0, 0, 600);
		setd(0, 10, rgb(0, r / 2 % 256, 0));
		parc(400 - r, 300 - r, 400 + r, 300 + r, 0, 600, 800, 0);
		setf(rgb(150, 100, 50));
		fpie(0, 0, r, r / 2, 0, 0, 800, 0);
		fpie(800 - r, 600 - r / 2, 800, 600, 800, 600, 0, 600);
		setd(0, 4, rgb(255, 122, 122));
		proundbar(800 - r, 0, 800, r / 2, r / 10, r / 20);
		proundbar(0, 600 - r / 2, r, 600, r / 10, r / 20);
		pbezier(0, 0, 800, 0, 0, 600, 800, 600);
		flushpaint();
		delay(1);
	}
}
*/
const int RS = (21*21*21)*3;
rect4<RS> s; int x_[RS], y_[RS];
string constr(long long s) {
	if (s < 10)return (string)"" + (char)(s + '0');
	return constr(s / 10) + (char)(s % 10 + '0');
}
void test3D() {
	int K = 0;
	ref(i, 0, 20)ref(j, 0, 20) if((i^j)&1){
		vector3 v((i - 10) * 50, (j - 10) * 50, -50);
		s.settriangle(K++, v + vector3(-25, -25, 50), v + vector3(-25, 25, 0), v + vector3(25, -25, 0), RGB(0, 127, 127));
		s.settriangle(K++, v + vector3(25, 25, 50), v + vector3(-25, 25, 0), v + vector3(25, -25, 0), RGB(127, 0, 127));
		s.settriangle(K++, v + vector3(25, -25, 0), v + vector3(75, 25, 0), v + vector3(25, 25, 50), RGB(127, 127, 0));
		s.settriangle(K++, v + vector3(-25, 25, 0), v + vector3(25, 75, 0), v + vector3(25, 25, 50), RGB(127, 127, 127));
	}
	trect4 tr = getidentity();
	int times = 0, rt = 0;
	time_t tt = time(0);
	while (++times) {
		peekmsg();
		if (GetAsyncKeyState(VK_ESCAPE))exit(0);
		if (islbuttondown())exit(0);
		tr = getidentity();
		if (GetAsyncKeyState(VK_LEFT)) { tr *= getmove(1, 0, 0); }
		if (GetAsyncKeyState(VK_RIGHT)) { tr *= getmove(-1, 0, 0); }
		if (GetAsyncKeyState(VK_UP)) {
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) tr *= getmove(0, 0, -1);
			else tr *= getmove(0, -1, 0);
		}
		if (GetAsyncKeyState(VK_DOWN)) {
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) tr *= getmove(0, 0, 1);
			else tr *= getmove(0, 1, 0);
		}
		if (GetAsyncKeyState('W')) { tr *= getrotate(-1, 0, 0, 0.01); }
		if (GetAsyncKeyState('S')) { tr *= getrotate(1, 0, 0, 0.01); }
		if (GetAsyncKeyState('A')) {
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) tr *= getrotate(0, 1, 0, 0.01);
			else tr *= getrotate(0, 0, 1, 0.006);
		}
		if (GetAsyncKeyState('D')) {
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) tr *= getrotate(0, -1, 0, 0.01);
			else tr *= getrotate(0, 0, -1, 0.006);
		}
		s *= tr;

		s.paint();

		sett(rgb(100, 100, 100), 40, 0, 0);
		ptext(100, 100, constr(rt).c_str());
		flushpaint();
		
		if (time(0) != tt) {
			tt = time(0);
			rt = times;
			times = 0;
		}
		
		delay(1);
	}
}

//WinMain函数
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevinstance,
	LPSTR lpCmdline,
	int nCmdshow
){
	//初始化
	if (checkprev("Hello World"))return 0;
	initwin(hInstance, 600,400, "Hello World");
	showwin(nCmdshow);
	
	//测试
	test3D();

}