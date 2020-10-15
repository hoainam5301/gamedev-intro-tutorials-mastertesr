#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject
{
public:
	int bottom_coll, created_item, bouncing;    //vachamday,taoitemroi,
	bool move;
	int width, height;
	//int cucgachranam;
	CBrick(float width, float height);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};