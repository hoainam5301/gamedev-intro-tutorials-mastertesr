#pragma once
#include "GameObject.h"
#include "Mario.h"
#define Mushroom	0
#define	Tree_Leaf	1
#define FIRE_FLOWER	2
#define Tree_Leaf_move_left 3
#define Tree_Leaf_move_right 4
#define SWITCH_P_ON	5
#define SWITCH_P_OFF 6
#define START_FLY_UP 7
#define ITEMS_END_GAME 8
#define GREEN_MUSHROOM 9

#define NAM_DUNG_YEN 0
#define NAM_DI_CHUYEN 0.05f

#define WIDTH_HEIGHT_ITEM 16
#define DISTANCE_MOVE_UP 15
#define MOVE_ITEM_UP_SPEED 0.05f
#define MOVING_SPEED 0.05f

#define DISTANCE_LEAF_MOVE_UP 20
#define DISTANCE_LEAF_CHANGE_DIC 35
#define MOVE_UPSPEED 0.45f
#define LEAF_MOVE_SPEED 0.3f
class CItems : public CGameObject
{
	
	
	bool spawn = true;
public:
	int change_direction = 1;
	float Start_y;
	float Start_x;
	float maxRight;
	vector <LPGAMEOBJECT> listEffect;
	bool move;
	bool hasPress;
	bool makeItemFly;
	//bool chamsan;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CItems(float start_x,float start_y);
	virtual void SetState(int state);
};

