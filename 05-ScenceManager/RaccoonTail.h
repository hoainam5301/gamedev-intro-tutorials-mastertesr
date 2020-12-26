#pragma once
#include "GameObject.h"

#define WIDTH_BBOX 16
#define HEIGHT_BBOX 6

class CRaccoonTail : public CGameObject
{

public:
	//CMario* Mario;
	bool isFighting;
	CRaccoonTail();
	void Collision_ENEMY(vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
};

