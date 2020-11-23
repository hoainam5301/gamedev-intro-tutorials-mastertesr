#include "Koopas.h"
#include "ColorBox.h"
#include "Floor.h"
#include "MonneyEffect.h"

CKoopas::CKoopas(CMario* mario)
{
	//CMario* Mario = new CMario();
	Mario = mario;
	SetState(KOOPA_RED_STATE_WALKING_RIGHT);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x+1;
	top = y+11;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPA_RED_STATE_DIE)
	{
		
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else if (state == KOOPA_RED_STATE_DIE_AND_MOVE || state == KOOPA_RED_STATE_DIE_AND_MOVE_UP)
	{
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else if (state == KOOPA_RED_STATE_REVIVE || state == KOOPA_RED_STATE_REVIVE_UP)
	{
		left = x;
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else if (state == KOOPA_RED_STATE_WALKING_RIGHT || state == KOOPA_RED_STATE_WALKING_LEFT)
	{
		top = y;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
	else if (state == KOOPA_RED_STATE_DIE_UP)
	{
		if (!hitbytail)
		{
			left = right;
			top = bottom;
		}
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else if (state == KOOPA_RED_STATE_HOLDING || state == KOOPA_RED_STATE_HOLDING_UP)
	{
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	}
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetState() == KOOPA_RED_STATE_HOLDING|| GetState()== KOOPA_RED_STATE_HOLDING_UP)
	{
		if (Mario->level == MARIO_RACCOON)
		{
			if (Mario->nx > 0)
				SetPosition(Mario->x + 17, Mario->y);
			else
				SetPosition(Mario->x - 12, Mario->y);
		}
		else if (Mario->level == MARIO_LEVEL_SMALL)
		{
			if (Mario->nx > 0)
				SetPosition(Mario->x + 19, Mario->y + 2);
			else
				SetPosition(Mario->x - 1, Mario->y +2);
		}
		else
		{
			if (Mario->nx > 0)
				SetPosition(Mario->x + 20, Mario->y);
			else
				SetPosition(Mario->x - 6, Mario->y);
		}
		//state = KOOPAS_ANI_DIE;
	}
	if (!Mario->isHolding && (last_state == KOOPA_RED_STATE_HOLDING||last_state==KOOPA_RED_STATE_HOLDING_UP))//de khi tha mai rua ra thi mai rua bi da
	{
		nx = Mario->nx;
		if (last_state == KOOPA_RED_STATE_HOLDING)
			SetState(KOOPA_RED_STATE_DIE_AND_MOVE);
		else if (last_state == KOOPA_RED_STATE_HOLDING_UP)
			SetState(KOOPA_RED_STATE_DIE_AND_MOVE_UP);
		
	}
	if (GetTickCount64() - timetorevive > 200000 && (last_state == KOOPA_RED_STATE_DIE || last_state == KOOPA_RED_STATE_DIE_UP || last_state==KOOPA_RED_STATE_HOLDING||last_state==KOOPA_RED_STATE_HOLDING_UP))//koopas vao trang thai chuan bi hoi sinh
	{
		Mario->isHolding = false;
		if (last_state == KOOPA_RED_STATE_DIE || last_state==KOOPA_RED_STATE_HOLDING)
			SetState(KOOPA_RED_STATE_REVIVE);
		else if (last_state == KOOPA_RED_STATE_DIE_UP || last_state==KOOPA_RED_STATE_HOLDING_UP)
			SetState(KOOPA_RED_STATE_REVIVE_UP);		
	}
	if (GetTickCount64() - timetorevive > 200000 && (last_state == KOOPA_RED_STATE_REVIVE || last_state==KOOPA_RED_STATE_REVIVE_UP ))
	{
		SetState(KOOPA_RED_STATE_WALKING_RIGHT);
		timetorevive = 0;		
	}
	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 
	if (makeEffect)
	{
		CMonneyEffect* monneyeffect = new CMonneyEffect();
		monneyeffect->SetPosition(x, y);
		monneyeffect->SetState(MAKE_100);
		makeEffect = false;
		listEffect.push_back(monneyeffect);
	}

	CGameObject::Update(dt);
	if(state!=KOOPA_RED_STATE_HOLDING && state!=KOOPA_RED_STATE_HOLDING_UP)
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
		if (ny != 0 && (state == KOOPA_RED_STATE_DIE_UP || state==KOOPA_RED_STATE_REVIVE_UP))
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
						SetState(KOOPA_RED_STATE_WALKING_RIGHT);
					}
					else if (x >= colorbox->x + 16 * colorbox->width - KOOPAS_BBOX_WIDTH)
					{
						x = colorbox->x + 16 * colorbox->width - KOOPAS_BBOX_WIDTH;
						SetState(KOOPA_RED_STATE_WALKING_LEFT);
					}
				}
				
			}
			else
			{
				if (e->nx != 0  )
				{
					vx = -vx;
					if (GetState() != KOOPA_RED_STATE_DIE_AND_MOVE && GetState() != KOOPA_RED_STATE_DIE_AND_MOVE_UP)
					{
						if (vx > 0)
							SetState(KOOPA_RED_STATE_WALKING_RIGHT);
						else
							SetState(KOOPA_RED_STATE_WALKING_LEFT);
					}
				}
			}
		}
		
	}
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Update(dt, coObjects);
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CKoopas::Render()
{
	animation_set->at(state)->Render(x, y);
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Render();
	}
	RenderBoundingBox();
}

void CKoopas::SetState(int State)
{
	CGameObject::SetState(State);   // last_state de xac dinh trang thai truoc do cua koopas roi thuc hien cac truong hop
	switch (State)
	{
	case KOOPA_RED_STATE_DIE:
		vx = 0;
		vy = 0;
		timetorevive = GetTickCount64();
		last_state = KOOPA_RED_STATE_DIE;
		break;
	case KOOPA_RED_STATE_DIE_UP:
		if (Mario->nx > 0)
			vx = 0.05;
		else
			vx = -0.05;
		vy = -0.05;
		timetorevive = GetTickCount64();
		last_state = KOOPA_RED_STATE_DIE_UP;
		break;
	case KOOPA_RED_STATE_WALKING_RIGHT:
		if (last_state == KOOPA_RED_STATE_REVIVE || last_state == KOOPA_RED_STATE_REVIVE_UP)
			y -= KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE;
		vx = KOOPAS_WALKING_SPEED;
		last_state = KOOPA_RED_STATE_WALKING_RIGHT;
		break;
	case KOOPA_RED_STATE_WALKING_LEFT:		
		vx = -KOOPAS_WALKING_SPEED;
		last_state = KOOPA_RED_STATE_WALKING_LEFT;
		break;
	case KOOPA_RED_STATE_DIE_AND_MOVE:
		last_state = KOOPA_RED_STATE_DIE_AND_MOVE;
		vx = 0.1 * nx;
		break;	
	case KOOPA_RED_STATE_REVIVE:
		last_state = KOOPA_RED_STATE_REVIVE;
		break;
	case KOOPA_RED_STATE_REVIVE_UP:
		last_state = KOOPA_RED_STATE_REVIVE_UP;
		break;
	case KOOPA_RED_STATE_DIE_AND_MOVE_UP:
		last_state = KOOPA_RED_STATE_DIE_AND_MOVE_UP;
		vx = 0.1 * nx;
		break;
	case KOOPA_RED_STATE_HOLDING:
		last_state = KOOPA_RED_STATE_HOLDING;
		break;
	case KOOPA_RED_STATE_HOLDING_UP:
		last_state = KOOPA_RED_STATE_HOLDING_UP;
		break;
	}

}