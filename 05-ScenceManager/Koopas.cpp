#include "Koopas.h"

CKoopas::CKoopas(CMario* mario)
{
	//CMario* Mario = new CMario();
	Mario = mario;
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
	{
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else if (state == KOOPAS_ANI_DIE_AND_MOVE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else if (state == KOOPAS_STATE_WALKING)
		bottom = top + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetState() == KOOPAS_STATE_DIE_AND_IS_HOLDING)
	{
		if (Mario->level == MARIO_RACCON)
		{
			if (Mario->nx > 0)
				SetPosition(Mario->x + 18, Mario->y + 9);
			else
				SetPosition(Mario->x - 12, Mario->y + 9);
		}
		else if (Mario->level == MARIO_LEVEL_SMALL)
		{
			if (Mario->nx > 0)
				SetPosition(Mario->x + 15, Mario->y -5);
			else
				SetPosition(Mario->x - 10, Mario->y -5);
		}
		else
		{
			if (Mario->nx > 0)
				SetPosition(Mario->x + 8, Mario->y + 9);
			else
				SetPosition(Mario->x - 10, Mario->y + 9);
		}
		//state = KOOPAS_ANI_DIE;
	}
	if (!Mario->isHolding && last_state == KOOPAS_STATE_DIE_AND_IS_HOLDING)
	{
		nx = Mario->nx;
		SetState(KOOPAS_ANI_DIE_AND_MOVE);
	}

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	//x += dx;
	//y += dy;
	/*if (state == KOOPAS_STATE_WALKING)
	{*/
	/*if (vx < 0 && x < 100) {
		x = 100; vx = -vx;
	}

	if (vx > 0 && x > 200) {
		x = 200; vx = -vx;
	}*/
	/*}
	else if (state == KOOPAS_ANI_DIE_AND_MOVE)
	{

	}*/


	CGameObject::Update(dt);
	if(state!=KOOPAS_STATE_DIE_AND_IS_HOLDING)
		vy += (0.00005f * dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		//chamsan = true;
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!

		x += min_tx * dx + nx * 0.5f;
		y += min_ty * dy + ny * 0.5f;

		if (ny != 0)
			vy = 0;
		//DebugOut(L"gia ti vx %f \n", vy);
		//if (coEvents.size() <= 3)
		//{
		//	x += dx;
		//}
		//else
		//	//doihuong *= -1;
		//	if (nx != 0) this->isdone = true;

		/*if (ny==0 && nx!=0)
		{
			doihuong *= -1;
			x += nx * 2;
		}*/



		//Collision logic with other objects

		/*for (UINT i = 0; i < coEventsResult.size(); i++)
		{

		}*/
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (vx > 0 && state != KOOPAS_ANI_DIE_AND_MOVE && state != KOOPAS_STATE_DIE_AND_IS_HOLDING) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0 && state != KOOPAS_ANI_DIE_AND_MOVE && state != KOOPAS_STATE_DIE_AND_IS_HOLDING) ani = KOOPAS_ANI_WALKING_LEFT;
	else if (state == KOOPAS_ANI_DIE_AND_MOVE) ani = KOOPAS_ANI_DIE_AND_MOVE;
	else if (state == KOOPAS_STATE_DIE_AND_IS_HOLDING) ani = KOOPAS_ANI_DIE;
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:

		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE;
		if (last_state == KOOPAS_ANI_DIE_AND_MOVE)
			y -= KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
		last_state = KOOPAS_STATE_DIE;
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
		last_state = KOOPAS_STATE_WALKING;
		break;
	case KOOPAS_ANI_DIE_AND_MOVE:
		last_state = KOOPAS_ANI_DIE_AND_MOVE;
		vx = 0.1 * nx;
		break;
	case KOOPAS_STATE_DIE_AND_IS_HOLDING:
		state = KOOPAS_ANI_DIE;
		last_state = KOOPAS_STATE_DIE_AND_IS_HOLDING;
		break;
	}

}