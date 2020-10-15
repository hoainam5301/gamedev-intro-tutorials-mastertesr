#pragma once
#include "GameObject.h"

#define SUPERMUSHROOM_WALKING_SPEED 0.05f

#define SUPERMUSHROOM_STATE_WALKING 100

#define SUPERMUSHROOM_BBOX_WIDTH  18
#define SUPERMUSHROOM_BBOX_HEIGHT 18

class CSuperMushroom : public CGameObject
{
public:
	int flag;
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void SetState(int state);
};

