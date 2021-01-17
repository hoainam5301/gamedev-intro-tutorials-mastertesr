#pragma once
#include "GameObject.h"

#define MAKE_100 0
#define MAKE_200 1
#define MAKE_400 2
#define MAKE_800 3
#define MAKE_1000 4
#define MAKE_2000 5
#define MAKE_4000 6
#define MAKE_8000 7
#define MAKE_ONE_UP 8
#define HIT_EFFECT 9

#define MOVING_SPEED_FORM_BRICK 0.001f
#define MOVE_SPEED_UP 0.1f
#define DISTANCE_MOVE 40

class CMonneyEffect : public CGameObject
{
public:
	CMonneyEffect();
	float start_y;
	ULONGLONG timeRenderHitEffect;
	bool coinFormBrick;  //do tien o cuc gach tao ra hieu ung thi di chuyen nhanh hon
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};

