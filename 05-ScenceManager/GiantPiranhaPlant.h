#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Fireball.h"

#define GIANT_BOX_WIDTH 16
#define  GIANT_BOX_HEIGHT 32
#define GIANT_GREEN_BOX_HEIGHT 24

#define GIANT_STATE_SHOOT_45_LEFT	0
#define GIANT_STATE_SHOOT_45_MORE_LEFT 1
#define GIANT_STATE_MOVE_DOWN_LEFT	2
#define	GIANT_STATE_MOVE_UP_LEFT	3
#define GIANT_STATE_SHOOT_45_RIGHT	4
#define GIANT_STATE_SHOOT_45_MORE_RIGHT 5
#define GIANT_STATE_MOVE_UP_RIGHT	6
#define GIANT_STATE_MOVE_DOWN_RIGHT	7

#define GIANT_GREEN_STATE_SHOOT_45_LEFT	8
#define GIANT_GREEN_STATE_SHOOT_45_MORE_LEFT 9
#define GIANT_GREEN_STATE_MOVE_DOWN_LEFT	10
#define	GIANT_GREEN_STATE_MOVE_UP_LEFT	11
#define GIANT_GREEN_STATE_SHOOT_45_RIGHT	12
#define GIANT_GREEN_STATE_SHOOT_45_MORE_RIGHT 13
#define GIANT_GREEN_STATE_MOVE_UP_RIGHT	14
#define GIANT_GREEN_STATE_MOVE_DOWN_RIGHT	15

#define GIANT_GREEN_STATE_BITE	16

#define GIANT_PIRANHA_RED 1
#define GIANT_PIRANHA_GREEN	2
#define GIANT_PIRANHA_BITE	3

class CGiantPiranhaPlant : public CGameObject
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
	bool fight;
	float delta_x, delta_y;
	CGiantPiranhaPlant(CMario* mario);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
};

