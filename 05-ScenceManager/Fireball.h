#pragma once
#include "GameObject.h"
#include "Mario.h"


class CFireball : public CGameObject
{
public:
	bool isbottom;
	bool istop;
	bool hasCollion;
	CMario* Mario;
	CFireball(D3DXVECTOR2 position, int nx,CMario* mario);
	void Collision_Mario(CMario* mario);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	~CFireball();
};



