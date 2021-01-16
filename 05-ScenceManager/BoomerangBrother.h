#pragma once
#include "GameObject.h"
#include "Mario.h"

#define BROTHER_STATE_MOVING 0
#define BROTHER_STATE_READY_TO_FIGHT 1
#define DISTANCE_GO_AROUND 50

#define  BROTHER_BBOX_WIDTH 16
#define	 BROTHER_BBOX_HEIGHT 24

class CBoomerangBrother : public CGameObject
{

public:
	int countBoomerang;
	int countBoomerangComeBack;
	bool isKill;
	float startX;
	ULONGLONG timereadyToThrow;
	vector <LPGAMEOBJECT> listBoomerang;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CheckCollisionWithBrick(vector<LPGAMEOBJECT>* coObjects);
	vector <LPGAMEOBJECT> listEffect;	
	CBoomerangBrother(CMario* mario);	
	CMario* Mario;
	virtual void SetState(int state);
};

