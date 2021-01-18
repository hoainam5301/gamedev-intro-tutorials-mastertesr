#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "BoomerangBrother.h"

#define BOOMERANG_STATE_MOVE_RIGHT	0
#define BOOMERANG_STATE_MOVE_LEFT	1
#define BOOMERANG_STATE_IDLE_RIGHT	2
#define BOOMERANG_STATE_IDLE_LEFT	3

#define BOOMERANG_BOX_WIDTH_HEIGH	16

#define BOOMERANG_DISTANCE_GO_UP 10
#define BOOMERANG_DISTANCE_GO_OUT 90

#define BOOMERANG_GRAVITY 0.00009f
#define BOOMERANG_SPEED_X 0.12f
#define BOOMERANG_ACC_TO_PULL_BACK 0.0002f
#define BOOMERANG_SPEED_Y 0.07f

class CBoomerang : public CGameObject
{
public:
	float startX=0;
	float startY=0;
	vector<LPGAMEOBJECT> listEffect;
	bool directionChanged=false;
	bool hasCollision = false;
	float limitPosY=0;
	int nxInit = 0;
	CMario* Mario;
	CBoomerangBrother* Brother;
	CBoomerang(int nx,CMario* mario,CBoomerangBrother* brother);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Render();
	virtual void SetState(int state);
	void Collision_Mario(CMario* mario);
	~CBoomerang();
};

