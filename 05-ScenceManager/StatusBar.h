#pragma once
#include "Sprites.h"
#include "Mario.h"
#include "GameObject.h"
#include <string>

#define SPEED_X_TIRER_1 0.1125f
#define SPEED_X_TIRER_2 0.125f
#define SPEED_X_TIRER_3 0.1375f
#define SPEED_X_TIRER_4 0.15f
#define SPEED_X_TIRER_5 0.1625f
#define SPEED_X_TIRER_6 0.175f
#define SPEED_X_TIRER_7 0.1875f
#define SPEED_X_TIRER_8 0.2f

#define Y 233
#define X 3

class StatusBar
{
	//vector<LPSPRITE> pMeter;
	LPSPRITE number;	
	int posX;
	int posY;
	int score;
	int money;
	int word;
	int life;
	int time;
	int second;
	void DrawBoard();
	void DrawNumber(int x, int y, string a,int maxsize);
	void DrawAllNumber();
	void DrawPMeter();
public:
	void Update(ULONGLONG dt, int camX, int camY);
	void Render();
	CMario* Mario;
	StatusBar(CMario* mario);
	~StatusBar();
};

