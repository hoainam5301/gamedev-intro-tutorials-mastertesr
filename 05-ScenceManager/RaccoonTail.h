#pragma once
#include "GameObject.h"
//#include "Mario.h"
class CRaccoonTail : public CGameObject
{

public:
	//CMario* Mario;
	CRaccoonTail();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

