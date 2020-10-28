#pragma once
#include "GameObject.h"
#include "Mario.h"
#define MOVING_SPEED 0.15
class CWeapon: public CGameObject
{
	bool spawn = true;
public:
	float start_x;
	float start_y;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CWeapon(float docaonhatdinh, float dorongnhatdinh,int marionx);
	//virtual void SetState(int state);
};

