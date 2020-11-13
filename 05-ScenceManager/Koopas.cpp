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
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	else if (state == KOOPAS_ANI_REVIVE)
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	else if (state == KOOPAS_STATE_WALKING)
		bottom = top + KOOPAS_BBOX_HEIGHT;
	else if (state == KOOPAS_ANI_DIE_UP)
	{
		left = right;
		top = bottom;
	}
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
				SetPosition(Mario->x + 10, Mario->y -5);
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
	if (GetTickCount() - timetorevive > 2000 && last_state==KOOPAS_STATE_DIE)
	{
		SetState(KOOPAS_ANI_REVIVE);
		//timetorevive = 0;
	}
	if (GetTickCount() - timetorevive > 4300 && last_state == KOOPAS_ANI_REVIVE)
	{
		SetState(KOOPAS_STATE_WALKING);
		timetorevive = 0;
	}
	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	//x += dx;
	//y += dy;
	/*if (state == KOOPAS_STATE_WALKING)
	{*/
	/*if (startx == 0)
		startx = x;
	if (vx < 0 && x < startx) {
		x = startx; vx = -vx;
		
	}

	if (vx > 0 && x > widthtogo && widthtogo!=0) {
		x = widthtogo; vx = -vx;
		
	}*/
	/*}
	else if (state == KOOPAS_ANI_DIE_AND_MOVE)
	{

	}*/


	CGameObject::Update(dt);
	if(state!=KOOPAS_STATE_DIE_AND_IS_HOLDING)
		vy += 0.00005f * dt;
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

		//x += min_tx * dx + nx * 0.5f;
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

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CColorBox*>(e->obj))
			{
				CColorBox* colorbox = dynamic_cast<CColorBox*>(e->obj);
				if (e->nx != 0)
				{					
					x += dx;
				}
				else if (e->ny < 0)
				{
					//startx = colorbox->x;
					//widthtogo = colorbox->x + 16 * (colorbox->width-1);
					if (x<=colorbox->x)
					{
						x = colorbox->x;
						vx = -vx;
					}
					else if (x >= colorbox->x + 16 * colorbox->width - KOOPAS_BBOX_WIDTH)
					{
						x = colorbox->x + 16 * colorbox->width - KOOPAS_BBOX_WIDTH;
						vx = -vx;
					}
				}
				
			}
			else
			{
				if (e->nx != 0)
				{
					vx = -vx;
				}
			}
		}
		//DebugOut(L"gia tri vx %f \n", vx);
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (vx > 0 && state==KOOPAS_STATE_WALKING ) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0 && state==KOOPAS_STATE_WALKING ) ani = KOOPAS_ANI_WALKING_LEFT;
	else if (state == KOOPAS_ANI_DIE_AND_MOVE) ani = KOOPAS_ANI_DIE_AND_MOVE;
	else if (state == KOOPAS_STATE_DIE_AND_IS_HOLDING) ani = KOOPAS_ANI_DIE;
	else if (state == KOOPAS_ANI_DIE_UP) ani = KOOPAS_ANI_DIE_UP;
	else if (state == KOOPAS_ANI_REVIVE) ani = KOOPAS_ANI_REVIVE;

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);   // last_state de xac dinh trang thai truoc do cua koopas roi thuc hien cac truong hop
	switch (state)
	{
	case KOOPAS_STATE_DIE:

		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE;
		if (last_state == KOOPAS_ANI_DIE_AND_MOVE)
			y -= KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
		timetorevive = GetTickCount();
		last_state = KOOPAS_STATE_DIE;
		break;
	case KOOPAS_STATE_WALKING:
		if (last_state == KOOPAS_ANI_REVIVE)
			y -= KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE;
		vx = KOOPAS_WALKING_SPEED;
		last_state = KOOPAS_STATE_WALKING;
		break;
	case KOOPAS_ANI_DIE_AND_MOVE:
		last_state = KOOPAS_ANI_DIE_AND_MOVE;
		vx = 0.1 * nx;
		break;
	case KOOPAS_STATE_DIE_AND_IS_HOLDING:
		if (last_state == KOOPAS_STATE_DIE)
			state = KOOPAS_ANI_DIE;
		else if (last_state == KOOPAS_ANI_DIE_UP)
			state = KOOPAS_ANI_DIE_UP;
		last_state = KOOPAS_STATE_DIE_AND_IS_HOLDING;
		break;
	case KOOPAS_ANI_DIE_UP:
		vy = -0.01;
		last_state = KOOPAS_ANI_DIE_UP;
		break;
	case KOOPAS_ANI_REVIVE:
		last_state = KOOPAS_ANI_REVIVE;
		break;
	case KOOPAS_ANI_REVIVE_UP:
		last_state = KOOPAS_ANI_REVIVE_UP;
		break;
	}

}