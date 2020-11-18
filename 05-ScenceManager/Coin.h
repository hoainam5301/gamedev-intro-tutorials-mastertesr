#pragma once
#include "GameObject.h"
class CCoin:public CGameObject
{
	
public:

	CCoin();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

