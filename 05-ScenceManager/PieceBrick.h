#pragma once
#include "GameObject.h"

#define MOVING_SPEED_X 0.07f
#define MOVING_SPEED_Y 0.16f
#define GRAVITY 0.0009f

class CPieceBrick : public CGameObject
{

	public:
	CPieceBrick(D3DXVECTOR2 position, int nx, int deflectFactorY = 1);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

