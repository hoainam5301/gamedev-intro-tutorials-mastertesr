#pragma once

#include "GameObject.h"
#include "Mario.h"
//#include "Floor.h"

#define KOOPAS_WALKING_SPEED 0.03f;

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 14

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_DIE_AND_IS_HOLDING 300

#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_DIE 2
#define KOOPAS_ANI_DIE_AND_MOVE 3
#define KOOPAS_ANI_DIE_UP	4
#define KOOPAS_ANI_REVIVE_UP 5
#define KOOPAS_ANI_REVIVE    6 

class CKoopas : public CGameObject
{

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	

public:
	DWORD timetorevive;
	int startx, widthtogo;
	CKoopas(CMario* mario);
	int last_state;
	CMario* Mario;
	virtual void SetState(int state);
};