#pragma once
#include "GameObject.h"
#include "Mario.h"

#define GIANT_BOX_WIDTH 16
#define  GIANT_BOX_HEIGHT 32

#define GIANT_STATE_SHOOT_45	0
#define GIANT_STATE_SHOOT_45_MORE 1
#define GIANT_STATE_MOVE_DOWN	2
#define	GIANT_STATE_MOVE_UP		3


class CGiantPiranhaPlant : public CGameObject
{
	int stateLocal;
public:
	//float  openclose;
	float start_x;			// initial position of Mario at scene
	float start_y;
	DWORD timewaittoshoot= GetTickCount64();
	DWORD timetomovedown;
	bool moveup = true;
	bool not_in_pipeline = false;
	bool fight;
	//CGiantPiranhaPlant(float x , float y );
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//void SetState(int state);
};

