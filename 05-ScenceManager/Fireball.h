#pragma once
#include "GameObject.h"


class CFireball : public CGameObject
{
public:
	CFireball(D3DXVECTOR2 position, int nx);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	~CFireball();
};



