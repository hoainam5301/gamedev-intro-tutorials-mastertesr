#pragma once
#include "GameObject.h"
#include "Mario.h"


class CMovingWood : public CGameObject
{
public:
	float width;
	float height;
	bool isFallingDown;
	CMario* Mario;
	ULONGLONG readyToFallingDown;
	CMovingWood(float width, float height,CMario* mario );
	virtual void Render();
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

