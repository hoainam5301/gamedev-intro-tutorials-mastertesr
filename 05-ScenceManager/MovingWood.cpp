#include "MovingWood.h"

CMovingWood::CMovingWood(float width, float height,CMario* mario)
{
	Mario = mario;
	this->width = width;
	this->height = height;
}

void CMovingWood::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);	
	//DebugOut(L"gia tri %f \n", x);
	if (!isFallingDown)
	{
		if (x - ((CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth())) < 50)
		{
			vx = -0.03;
		}
	}
	else
	{
		if (GetTickCount64() - readyToFallingDown > 30 && readyToFallingDown != 0)
		{
			vx = 0;
			vy += 0.0006f*dt;
			//isFallingDown = false;
		}
	} 
	x += dx;
	y += dy;
	
	//DebugOut(L"aa123123123");
}


void CMovingWood::Render()
{	
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CMovingWood::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x +width*16;
	b = y + height*16;
}