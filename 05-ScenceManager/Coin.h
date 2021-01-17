#pragma once
#include "GameObject.h"

#define COIN_STATE_ROTATE 0
#define  COIN_STATE_NO_ROTATE 2
#define COIN_STATE_CREATED 1

#define MOVE_SPEED_COIN 0.0025f

#define DISTANCE_MOVE_UP_COIN 60
#define DISTANCE_MOVE_DOWN_COIN 20

#define WIDTH_HEIGHT_COIN 16

class CCoin:public CGameObject
{
	
public:

	CCoin();
	float start_y;
	bool spawn = true;
	bool tranformation;
	bool hasCreatMoneyEffect;
	vector <LPGAMEOBJECT> listEffect;
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};



