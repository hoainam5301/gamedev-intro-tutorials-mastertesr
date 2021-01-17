#pragma once
#include "GameObject.h"
#include "Mario.h"
//#include "ColorBox.h"

#define GOOMBA_WALKING_SPEED 0.05f;
#define GOOMBA_FLY_HIGH_SPEED 0.15f
#define GOOMBA_FLY_LOW_SPEED 0.05f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING 0
#define GOOMBA_STATE_DIE 1
#define GOOMBA_STATE_DIE_FLY 2

#define GOOMBA_RED_STATE_HAS_WING_WALK 3
#define GOOMBA_RED_STATE_HAS_WING_FLY_LOW 4
#define GOOMBA_RED_STATE_HAS_WING_FLY_HIGH 5
#define GOOMBA_RED_STATE_NO_WING_WALK 6
#define GOOMBA_RED_STATE_NO_WING_DIE 7
#define GOOMBA_RED_STATE_NO_WING_DIE_FLY 8

#define GOOMBA_NORMAL 1
#define GOOMBA_RED 2

#define TIME_READY_TO_SULKY 5000
#define	TIME_RENDER_ANI_DIE 300
#define TIME_READY_TO_FLY_HIGH	1000
#define TIME_TO_FLY_LOW_AND_HIGH	200
#define FLY_HIGH 4

class CGoomba : public CGameObject
{
	
public: 
	bool hasWing=true;
	bool isGrounded;
	bool isKill;
	ULONGLONG sulkyMario;  //doi mario bo ko them duoi nua
	CMario* Mario;
	ULONGLONG readyToFlyHigh;
	int countFly;
	bool hitByWeapon;
	bool hitByTail;
	vector <LPGAMEOBJECT> listEffect;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetSpeed();
	CGoomba(float width, float height);
	void SetDirection();
	ULONGLONG timeRenderAniDie;
	CGoomba(CMario* mario);
	virtual void SetState(int state);
};