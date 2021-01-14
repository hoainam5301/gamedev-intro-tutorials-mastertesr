#pragma once

#include "GameObject.h"
#include "Mario.h"


#define KOOPAS_WALKING_SPEED 0.03f
#define KOOPAS_MOVING_SPEED 0.1f
#define KOOPAS_FLYING_SPEED 0.18f
#define GARVITY 0.0005f

#define KOOPAS_BBOX_WIDTH 17
#define KOOPAS_BBOX_HEIGHT 27
#define KOOPAS_BBOX_HEIGHT_DIE 14

#define TIME_TO_READY_REVIVE 8000
#define TIME_REVIVE 10000
#define TIME_READY_FLY 900


#define KOOPA_RED 1
#define KOOPA_GREEN 2

#define KOOPA_RED_STATE_WALKING_LEFT	 0
#define KOOPA_RED_STATE_WALKING_RIGHT	 1
#define KOOPA_RED_STATE_DIE				 2
#define KOOPA_RED_STATE_DIE_AND_MOVE	 3
#define KOOPA_RED_STATE_DIE_UP			 4
#define KOOPA_RED_STATE_REVIVE_UP		 5
#define KOOPA_RED_STATE_REVIVE			 6 
#define KOOPA_RED_STATE_DIE_AND_MOVE_UP	 7
#define KOOPA_RED_STATE_HOLDING		 	 8
#define KOOPA_RED_STATE_HOLDING_UP       9
#define KOOPA_RED_STATE_HAS_WING_WALKING_LEFT 22

#define KOOPA_GREEN_STATE_WALKING_LEFT	 10
#define KOOPA_GREEN_STATE_WALKING_RIGHT  11
#define KOOPA_GREEN_STATE_DIE			 12
#define KOOPA_GREEN_STATE_DIE_AND_MOVE	 13
#define KOOPA_GREEN_STATE_REVIVE	     14 
#define KOOPA_GREEN_STATE_DIE_UP		 15
#define KOOPA_GREEN_STATE_DIE_AND_MOVE_UP 16
#define KOOPA_GREEN_STATE_REVIVE_UP		 17
#define KOOPA_GREEN_STATE_HOLDING		 18
#define KOOPA_GREEN_STATE_HOLDING_UP	 19
#define KOOPA_GREEN_STATE_HAS_WING_FLY_LEFT	20
#define KOOPA_GREEN_STATE_HAS_WING_FLY_RIGHT 21

class CKoopas : public CGameObject
{

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CheckCollisionWithBrick(vector<LPGAMEOBJECT>* coObjects);
	vector <LPGAMEOBJECT> listEffect;
	ULONGLONG timeToRevive;
	ULONGLONG timeToFly;
	ULONGLONG timeRenderAniDie;
	bool hitByTail;
	bool hasWing;
	bool hitByWeapon;
	int startx, widthtogo;
	float startX, startY;
	CKoopas(CMario* mario,int id_Koopa,int hasWing);
	int last_state;
	CMario* Mario;
	virtual void SetState(int state);
};