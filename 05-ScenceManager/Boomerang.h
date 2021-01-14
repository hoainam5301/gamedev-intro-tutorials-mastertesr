#pragma once
#include "GameObject.h"
#include "BoomerangBrother.h"
#include "Mario.h"

#define WIDTH_HEIGHT_BOOMERANG 16

class CBoomerang : public CGameObject
{
public:
	CMario* Mario;
	float startX;
	float startY;
	CBoomerang(int nx, CMario* mario,CBoomerangBrother* brother);
	CBoomerangBrother* Brother;
	void Collision_Mario(CMario* mario);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	~CBoomerang();

};

