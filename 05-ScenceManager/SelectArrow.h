#pragma once
#include "GameObject.h"
#include "Mario.h"
class CSelectArrow : public CGameObject
{
public:
	float startX, startY;
	CMario* Mario;
	CSelectArrow(CMario* mario);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	virtual void Render();
};

