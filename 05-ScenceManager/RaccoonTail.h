#pragma once
#include "GameObject.h"
//#include "Mario.h"
class CRaccoonTail : public CGameObject
{

public:
	//CMario* Mario;
	bool isFighting;
	CRaccoonTail();
	void Collision_ENEMY(vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};

