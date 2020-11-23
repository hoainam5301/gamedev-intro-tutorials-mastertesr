#pragma once
#include "GameObject.h"

#define MAKE_100 0
#define MAKE_200 1
#define MAKE_400 2
#define MAKE_800 3
#define MAKE_1000 4
#define MAKE_2000 5
#define MAKE_4000 6
#define MAKE_8000 7

class CMonneyEffect : public CGameObject
{
public:
	CMonneyEffect();
	float start_y;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};

