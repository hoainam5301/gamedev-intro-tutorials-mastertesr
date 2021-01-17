#pragma once
#include "GameObject.h"
#include "Mario.h"

#define BONUS_POS_X 10
#define BONUS_POS_XY 6

#define MOVE_SPEED_X 0.1f
#define MOVE_SPEED_Y 0.08f

#define WIDTH_HEIGHT_FIREBALL 8


class CFireball : public CGameObject
{
public:
	bool isbottom=false;
	bool istop=false;
	bool hasCollion=false;
	ULONGLONG timeRenderFireExplore=0;
	CMario* Mario;
	CFireball(D3DXVECTOR2 position, int nx,CMario* mario);
	void Collision_Mario(CMario* mario);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	~CFireball();
};



