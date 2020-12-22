#pragma once
#include "GameObject.h"
class CShakeTree : public CGameObject
{

public:
	CShakeTree();
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	~CShakeTree();
};

