#pragma once
#include "GameObject.h"

#define COIN_STATE_ROTATE 0
#define  COIN_STATE_NO_ROTATE 2
#define COIN_STATE_CREATED 1

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

class Coin
{
};

