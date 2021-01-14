#pragma once
#include "GameObject.h"

#define BBOX_WIDTH  16
#define BBOX_HEIGHT 16

class CFloor : public CGameObject
{
	int height;
	float width;
public:

	CFloor(float width, int height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
