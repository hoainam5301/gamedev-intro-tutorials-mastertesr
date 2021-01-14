#pragma once
#include "GameObject.h"
#include "Mario.h"

#define GIANT_BBOX_WIDTH 16
#define GIANT_BBOX_HEIGHT 24
#define MOVE_SPEED 0.15f

#define GIANT_STATE_MOVE_UP 0
#define GIANT_STATE_MOVE_DOWN 1

#define TIME_MOVE_UP	1500
#define TIME_MOVE_DOWN	3000


class CGiantPiranhaPlantBite : public CGameObject
{
public:
	float start_x;			// initial position of Mario at scene
	float start_y;
	vector <LPGAMEOBJECT> listFireBall;
	ULONGLONG timewaittoshoot = GetTickCount64();
	//ULONGLONG timetomovedown;
	CMario* Mario;
	bool moveup = true;
	//bool not_in_pipeline = false;
	//bool fight;
	//float delta_x, delta_y;
	CGiantPiranhaPlantBite(CMario* mario);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

};


