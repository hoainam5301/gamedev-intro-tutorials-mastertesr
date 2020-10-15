#pragma once
#include "GameObject.h"

#define GIANT_BOX_WIDTH 16
#define  GIANT_BOX_HEIGHT 16

#define	GIANT_STATE_CLOSE 0
#define GIANT_STATE_OPEN  1

#define GIANT_ANI_CLOSE 0
#define GIANT_ANI_OPEN 1

class CGiantPiranhaPlant : public CGameObject
{
	int stateLocal;
public:
	//float  openclose;
	float start_x;			// initial position of Mario at scene
	float start_y;
	CGiantPiranhaPlant(float x , float y );
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//void SetState(int state);
};

