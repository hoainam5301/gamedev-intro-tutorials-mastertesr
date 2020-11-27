#pragma once
#include "GameObject.h"
class CCoin:public CGameObject
{
	
public:

	CCoin();
	float start_y;
	bool spawn = true;
	vector <LPGAMEOBJECT> listEffect;
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

