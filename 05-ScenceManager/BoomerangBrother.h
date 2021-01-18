#pragma once
#include "GameObject.h"
#include "Mario.h"

#define BROTHER_STATE_IDLE_RIGHT	0
#define BROTHER_STATE_MOVE_RIGHT	1
#define BROTHER_STATE_ATTACK_RIGHT	2
#define BROTHER_STATE_DIE_RIGHT		3
#define BROTHER_STATE_JUMP_RIGHT	8

#define BROTHER_STATE_IDLE_LEFT		4
#define BROTHER_STATE_MOVE_LEFT		5
#define BROTHER_STATE_ATTACK_LEFT	6
#define BROTHER_STATE_DIE_LEFT		7
#define BROTHER_STATE_JUMP_LEFT		9

#define BROTHER_BBOX_WIDTH 16
#define BROTHER_BBOX_HEIGHT	24

#define BROTHER_GRAVITY	0.0005f
#define BROTHER_SPEED_GO_AROUND 0.035f
#define BROTHER_JUMP_SPEED	0.14f
#define DISTANCE_BROTHER_GO 25



class CBoomerangBrother : public CGameObject
{


public:
	vector<LPGAMEOBJECT> listBoomerang;
	vector<LPGAMEOBJECT> listEffect;
	ULONGLONG timereadyToThrow=0;
	ULONGLONG timeToRelax = 0;
	ULONGLONG timeRenderDie=0;
	int countBoomerang;
	int randTurnToJump=0;
	int countToJump=0;
	float startX=0;
	float startY=0;
	bool isHoldingBoomerang=false;
	bool died=false;
	bool isGrounded;
	CMario* Mario;
	CBoomerangBrother(CMario* mario);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	void CreateBoomerang();
	void GenerateRandomTurnToJump();
	int GetPlayerDirection();

	~CBoomerangBrother();
};

