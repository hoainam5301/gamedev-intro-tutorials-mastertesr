#include "Koopas.h"
#include "ColorBox.h"
#include "Floor.h"

CKoopas::CKoopas(CMario* mario)
{
	//CMario* Mario = new CMario();
	Mario = mario;
	SetState(KOOPAS_STATE_WALKING_RIGHT);
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
	else if (state == KOOPAS_STATE_DIE_AND_MOVE||state==KOOPAS_STATE_DIE_AND_MOVE_UP)
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	else if (state == KOOPAS_STATE_REVIVE || state==KOOPAS_STATE_REVIVE_UP)
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	else if (state == KOOPAS_STATE_WALKING_RIGHT ||state==KOOPAS_STATE_WALKING_LEFT)
		bottom = top + KOOPAS_BBOX_HEIGHT;
	else if (state == KOOPAS_STATE_DIE_UP)
	{
		if (!hitbytail)
		{
			left = right;
			top = bottom;
		}
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else if (state == KOOPAS_STATE_HOLDING||state==KOOPAS_STATE_HOLDING_UP)
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetState() == KOOPAS_STATE_HOLDING|| GetState()== KOOPAS_STATE_HOLDING_UP)
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
	if (!Mario->isHolding && (last_state == KOOPAS_STATE_HOLDING||last_state==KOOPAS_STATE_HOLDING_UP))//de khi tha mai rua ra thi mai rua bi da
	{
		nx = Mario->nx;
		if (last_state == KOOPAS_STATE_HOLDING)
			SetState(KOOPAS_STATE_DIE_AND_MOVE);
		else if (last_state == KOOPAS_STATE_HOLDING_UP)
			SetState(KOOPAS_STATE_DIE_AND_MOVE_UP);
		
	}
	if (GetTickCount() - timetorevive > 4000 && (last_state == KOOPAS_STATE_DIE || last_state == KOOPAS_STATE_DIE_UP || last_state==KOOPAS_STATE_HOLDING||last_state==KOOPAS_STATE_HOLDING_UP))//koopas vao trang thai chuan bi hoi sinh
	{
		Mario->isHolding = false;
		if (last_state == KOOPAS_STATE_DIE || last_state==KOOPAS_STATE_HOLDING)
			SetState(KOOPAS_STATE_REVIVE);
		else if (last_state == KOOPAS_STATE_DIE_UP || last_state==KOOPAS_STATE_HOLDING_UP)
			SetState(KOOPAS_STATE_REVIVE_UP);		
	}
	if (GetTickCount() - timetorevive > 5000 && (last_state == KOOPAS_STATE_REVIVE || last_state==KOOPAS_STATE_REVIVE_UP ))
	{
		SetState(KOOPAS_STATE_WALKING_RIGHT);
		timetorevive = 0;		
	}
	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	CGameObject::Update(dt);
	if(state!=KOOPAS_STATE_HOLDING && state!=KOOPAS_STATE_HOLDING_UP)
		vy += 0.0005f * dt;
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
		if (ny != 0 && (state == KOOPAS_STATE_DIE_UP || state==KOOPAS_STATE_REVIVE_UP))
		{
			vx = 0;
		}
	
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
					if (x<=colorbox->x)
					{
						x = colorbox->x;
						SetState(KOOPAS_STATE_WALKING_RIGHT);
					}
					else if (x >= colorbox->x + 16 * colorbox->width - KOOPAS_BBOX_WIDTH)
					{
						x = colorbox->x + 16 * colorbox->width - KOOPAS_BBOX_WIDTH;
						SetState(KOOPAS_STATE_WALKING_LEFT);
					}
				}
				
			}
			else
			{
				if (e->nx != 0  )
				{
					vx = -vx;
					if (GetState() != KOOPAS_STATE_DIE_AND_MOVE && GetState() != KOOPAS_STATE_DIE_AND_MOVE_UP)
					{
						if (vx > 0)
							SetState(KOOPAS_STATE_WALKING_RIGHT);
						else
							SetState(KOOPAS_STATE_WALKING_LEFT);
					}
				}
			}
		}
		
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CKoopas::Render()
{
	animation_set->at(state)->Render(x, y);
	RenderBoundingBox();
}

void CKoopas::SetState(int State)
{
	CGameObject::SetState(State);   // last_state de xac dinh trang thai truoc do cua koopas roi thuc hien cac truong hop
	switch (State)
	{
	case KOOPAS_STATE_DIE:
		vx = 0;
		vy = 0;
		timetorevive = GetTickCount();
		last_state = KOOPAS_STATE_DIE;
		break;
	case KOOPAS_STATE_DIE_UP:
		if (Mario->nx > 0)
			vx = 0.05;
		else
			vx = -0.05;
		vy = -0.05;
		timetorevive = GetTickCount();
		last_state = KOOPAS_STATE_DIE_UP;
		break;
	case KOOPAS_STATE_WALKING_RIGHT:
		if (last_state == KOOPAS_STATE_REVIVE || last_state == KOOPAS_STATE_REVIVE_UP)
			y -= KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE;
		vx = KOOPAS_WALKING_SPEED;
		last_state = KOOPAS_STATE_WALKING_RIGHT;
		break;
	case KOOPAS_STATE_WALKING_LEFT:		
		vx = -KOOPAS_WALKING_SPEED;
		last_state = KOOPAS_STATE_WALKING_LEFT;
		break;
	case KOOPAS_STATE_DIE_AND_MOVE:
		last_state = KOOPAS_STATE_DIE_AND_MOVE;
		vx = 0.1 * nx;
		break;	
	case KOOPAS_STATE_REVIVE:
		last_state = KOOPAS_STATE_REVIVE;
		break;
	case KOOPAS_STATE_REVIVE_UP:
		last_state = KOOPAS_STATE_REVIVE_UP;
		break;
	case KOOPAS_STATE_DIE_AND_MOVE_UP:
		last_state = KOOPAS_STATE_DIE_AND_MOVE_UP;
		vx = 0.1 * nx;
		break;
	case KOOPAS_STATE_HOLDING:
		last_state = KOOPAS_STATE_HOLDING;
		break;
	case KOOPAS_STATE_HOLDING_UP:
		last_state = KOOPAS_STATE_HOLDING_UP;
		break;
	}

}