#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define GRAVITY 0.3f
#define MOVE_UP_SPEED 0.05f
#define BRICK_AFTER 1
#define QUESTION_BRICK 0
#define BRICK_MAKE_LIFE 2

#define	ID_GACH_RA_ITEMS	2
#define ID_GACH_RA_TIEN		1
#define ID_GACH_SWITCH_P    3
#define ID_GACH_BONUS		4
#define ID_GACH_RA_NHIEU_TIEN 5
#define ID_GACH_RA_LEAF_TREA 6
#define BRICK_MOVE_UP 8 //do cao cuc gay nay len
#define BRICK_MOVE_DOWN 72//vi tri cuc gach sau khi roi xuong lai

class CBrick : public CGameObject
{
public:
	int bottom_coll, created_item, bouncing;    //vachamday,taoitemroi,
	bool collWithKoopa;
	bool move;
	int width, height;
	bool hitByTail;	
	int countMoneyHasMake;
	CBrick(float width, float height);
	virtual void Render();
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};