#pragma once

#include "GameObject.h"
#include "Mario.h"


#define KOOPAS_WALKING_SPEED 0.03f;

#define KOOPAS_BBOX_WIDTH 17
#define KOOPAS_BBOX_HEIGHT 27
#define KOOPAS_BBOX_HEIGHT_DIE 16



#define KOOPAS_STATE_WALKING_LEFT 0
#define KOOPAS_STATE_WALKING_RIGHT 1
#define KOOPAS_STATE_DIE 2
#define KOOPAS_STATE_DIE_AND_MOVE 3
#define KOOPAS_STATE_DIE_UP	4
#define KOOPAS_STATE_REVIVE_UP 5
#define KOOPAS_STATE_REVIVE    6 
#define KOOPAS_STATE_DIE_AND_MOVE_UP 7
#define KOOPAS_STATE_HOLDING	8
#define KOOPAS_STATE_HOLDING_UP 9

class CKoopas : public CGameObject
{

	
	

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	DWORD timetorevive;
	bool hitbytail;
	int startx, widthtogo;
	CKoopas(CMario* mario);
	int last_state;
	CMario* Mario;
	virtual void SetState(int state);
};