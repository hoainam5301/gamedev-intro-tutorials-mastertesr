#include "RaccoonTail.h"

#include "BrokenBrick.h"


CRaccoonTail::CRaccoonTail()
{
	 //Mario = mario;
}

void CRaccoonTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{	
	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 
	CGameObject::Update(dt);
	Collision_ENEMY(coObjects);
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

	//	//x += min_tx * dx + nx * 0.5f;
	//	//y += min_ty * dy + ny * 0.5f;

	//	if (ny != 0)
	//		vy = 0;
	//	//Collision logic with other objects
	//	if (isFighting)
	//	{
	//		for (UINT i = 0; i < coEventsResult.size(); i++)
	//		{
	//			LPCOLLISIONEVENT e = coEventsResult[i];


	//			if (dynamic_cast<CBrokenBrick*>(e->obj))
	//			{
	//				CBrokenBrick* brokenbrick = dynamic_cast<CBrokenBrick*>(e->obj);
	//				if (e->nx != 0)
	//				{
	//					brokenbrick->isDestroyed = true;
	//					brokenbrick->SetState(STATE_DESTROYED);
	//				}

	//			}

	//		}
	//	}
	//	//DebugOut(L"gia tri vx %f \n", vx);
	//}

	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CRaccoonTail::Collision_ENEMY(vector<LPGAMEOBJECT>* coObjects)
{
	float l_ene, t_ene, r_ene, b_ene, l_tail, t_tail, r_tail, b_tail;
	GetBoundingBox(l_tail, t_tail, r_tail, b_tail);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (dynamic_cast<CBrokenBrick*>(e))
		{
			CBrokenBrick* brokenbrick = dynamic_cast<CBrokenBrick*>(e);
			brokenbrick->GetBoundingBox(l_ene, t_ene, r_ene, b_ene);
			if (CGameObject::CheckAABB(l_ene , t_ene , r_ene , b_ene , l_tail, t_tail, r_tail, b_tail))
			{
				brokenbrick->isDestroyed = true;
				brokenbrick->SetState(STATE_DESTROYED);
			}
		}
		//else if(dynamic_cast<C>)
	}
}

void CRaccoonTail::Render()
{
	RenderBoundingBox();
}

void CRaccoonTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y ;
	r =	x+16;
	b = y+6 ;
}
