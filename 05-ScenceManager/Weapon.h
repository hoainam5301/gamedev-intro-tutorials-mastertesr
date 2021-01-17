#pragma once
#include "GameObject.h"
#include "Mario.h"
#define MOVING_SPEED_WEAPON 0.15

#define FIRE_BALL_MOVE 0
#define FIRE_BALL_EXPLODE 1
class CWeapon: public CGameObject
{
	bool spawn = true;
public:
	bool isWaitingForAni;
	//bool isExplode;
	float start_x;
	float start_y;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	CWeapon(float docaonhatdinh, float dorongnhatdinh,int marionx);
	void CheckCollision(vector<LPGAMEOBJECT>* cocoObjects);
	//virtual void SetState(int state);
};

