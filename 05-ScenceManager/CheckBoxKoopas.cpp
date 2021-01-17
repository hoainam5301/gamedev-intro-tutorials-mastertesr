#include "CheckBoxKoopas.h"
#include "BrokenBrick.h"
#include "Brick.h"


CCheckBoxKoopas::CCheckBoxKoopas()
{
	
	
}

void CCheckBoxKoopas::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (startY == 0)
		startY = y;
	vy += GRAVATY_OF_BOX * dt;
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		makeKoopaReturn = true;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		
		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.5f;
		y += min_ty * dy + ny * 0.01f;
		
		if (ny != 0)
		{
			vy = 0;
			makeKoopaReturn = false;
		}
		
		/*if (ny != 0 && (state == KOOPA_RED_STATE_DIE_UP || state == KOOPA_RED_STATE_REVIVE_UP || state == KOOPA_GREEN_STATE_REVIVE_UP || state == KOOPA_GREEN_STATE_DIE_UP))
		{
			vx = 0;
		}*/

		//Collision logic with other objects
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			/*LPCOLLISIONEVENT e = coEventsResult[i];
			if(dynamic_cast())*/

		}
	}
	
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	
}

void CCheckBoxKoopas::Render()
{
	//DebugOut(L"hiiii\n");
	//animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CCheckBoxKoopas::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x + 1;
	t = y ;
	b = t + 27;
	r = x + 17;
}
