#pragma once
#include "GameObject.h"
#define GRAVATY_OF_BOX 0.0001f

class CCheckBoxKoopas : public CGameObject
{

	int height;
	float width;
public:
	float startY;
	bool makeKoopaReturn;
	CCheckBoxKoopas();
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

