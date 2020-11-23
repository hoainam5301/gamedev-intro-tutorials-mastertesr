#pragma once
#include "GameObject.h"
#include "Mario.h"
//#include "ColorBox.h"

#define GOOMBA_WALKING_SPEED 0.05f;

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

class CGoomba : public CGameObject
{
	
public: 
	bool hasWing=true;
	bool isGrounded;
	DWORD sulkyMario;  //doi mario bo ko them duoi nua
	CMario* Mario;
	DWORD readyToFlyHigh;
	int countFly;
	vector <LPGAMEOBJECT> listEffect;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetSpeed();
	CGoomba(float width, float height);
	DWORD timeRenderAniDie;
	CGoomba(CMario* mario);
	virtual void SetState(int state);
};