#pragma once
#include "GameObject.h"
#include "Mario.h"
class CWeapon: public CGameObject
{
	bool spawn = true;
public:
	//int doihuong = 1;
	float start_x;
	float start_y;
	//bool chamsan;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	CWeapon(float docaonhatdinh, float dorongnhatdinh,CMario* mario);
	//virtual void SetState(int state);
};

