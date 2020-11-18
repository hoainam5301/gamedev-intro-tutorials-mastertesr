#pragma once
#include "GameObject.h"
#include "Brick.h"
//#include "Mario.h"

#define Mushroom	0
#define	Tree_Leaf	1
#define FIRE_FLOWER	2

#define NAM_DUNG_YEN 0
#define NAM_DI_CHUYEN 0.05f
class CItems : public CGameObject
{
	
	
	bool spawn = true;
public:
	int change_direction = 1;
	float Start_y;
	float Start_x;
	bool move;
	//bool chamsan;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CItems(float start_x,float start_y);
	virtual void SetState(int state);
};

