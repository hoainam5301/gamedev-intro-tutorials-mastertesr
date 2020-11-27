#pragma once
#include "GameObject.h"


class CFireball : public CGameObject
{
public:
	bool isbottom;
	bool istop;
	CFireball(D3DXVECTOR2 position, int nx);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	~CFireball();
};



