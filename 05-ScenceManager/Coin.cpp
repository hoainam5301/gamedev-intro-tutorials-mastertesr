#include "Coin.h"

CCoin::CCoin()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(13));
}
void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vx = 0;
	vy = 0;
	DebugOut(L"gia tri vx %f \n", vx);
	
	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();
	//CalcPotentialCollisions(coObjects, coEvents);

	//if (coEvents.size() == 0)
	//{
	//	x += dx;
	//	y += dy;
	//}
	//else
	//{
	//	float min_tx, min_ty, nx = 0, ny = 0;
	//	float rdx = 0;
	//	float rdy = 0;

	//	// TODO: This is a very ugly designed function!!!!
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	//	// block every object first!
	//	y += min_ty * dy + ny * 0.5f;
	//	x += min_tx * dx + nx * 0.5f;

	//	if (ny < 0)
	//	{
	//		vy = -0.1;
	//	}

	//	//
	//	// Collision logic with other objects
	//	//
	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];

	//		
	//		
	//	}
	//}

	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*left = x;
	top = y;
	right = x + 8;
	bottom = y + 8;*/
}

void CCoin::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}



