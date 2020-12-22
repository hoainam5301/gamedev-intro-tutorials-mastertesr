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


#define NAM_DUNG_YEN 0
#define NAM_DI_CHUYEN 0.05f
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
	//bool chamsan;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CItems(float start_x,float start_y);
	virtual void SetState(int state);
};

