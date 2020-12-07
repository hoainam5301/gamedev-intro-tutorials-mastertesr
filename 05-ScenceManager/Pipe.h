#pragma once
#include "GameObject.h"
class CPipe : public CGameObject
{

public:
	int height;
	int width;
	int pypeType;
	CPipe(int width, int height,int pipetype);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

