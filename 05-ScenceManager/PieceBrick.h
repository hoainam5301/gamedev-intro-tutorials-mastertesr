#pragma once
#include "GameObject.h"
class CPieceBrick : public CGameObject
{

	public:
	CPieceBrick(D3DXVECTOR2 position, int nx, int deflectFactorY = 1);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

