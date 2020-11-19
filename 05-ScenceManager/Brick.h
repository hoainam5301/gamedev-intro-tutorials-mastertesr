#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_AFTER 1
#define QUESTION_BRICK 0
#define GRAVITY 0.3f
#define	ID_GACH_RA_ITEMS	2
#define BRICK_MOVE_UP 8 //do cao cuc gay nay len
#define BRICK_MOVE_DOWN 72//vi tri cuc gach sau khi roi xuong lai

class CBrick : public CGameObject
{
public:
	int bottom_coll, created_item, bouncing;    //vachamday,taoitemroi,
	bool move;
	int width, height;
	bool hitbytail;	
	CBrick(float width, float height);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};