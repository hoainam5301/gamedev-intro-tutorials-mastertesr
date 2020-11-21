#include "Goomba.h"
#include "ColorBox.h"

CGoomba::CGoomba()
{
	if (id_goomba == GOOMBA_NORMAL)
		SetState(GOOMBA_STATE_WALKING);
	else if (id_goomba == GOOMBA_RED)
	{
		SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
		readytohighfly = GetTickCount64();
	}
	
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	DebugOut(L"id gooooo %d \n", id_goomba);
	if (id_goomba == GOOMBA_NORMAL)
	{
		if (state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DIE_FLY)
		{
			left = x;
			top = y;
			right = x + GOOMBA_BBOX_WIDTH;
			bottom = y + GOOMBA_BBOX_HEIGHT;
		}
	}
	else if (id_goomba == GOOMBA_RED)
	{
		if (state != GOOMBA_RED_STATE_NO_WING_DIE && state != GOOMBA_RED_STATE_NO_WING_DIE_FLY)
		{
			if (haswing)
			{
				left = x;
				top = y;
				right = x + 20;
				bottom = y + 24;
			}
			else
			{
				left = x;
				top = y;
				right = x + 26;
				bottom = y + 25;
			}
			
		}
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 	
	//DebugOut(L"id goomba %d \n",id_goomba);
	if (id_goomba == 2)
	{
		if (haswing)
		{
			if (countfly == 0 && isGrounded && GetTickCount64()-readytohighfly>1000)
			{
				//DebugOut(L"aaaa \n");
				SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
				readytohighfly = GetTickCount64();
				countfly = 1;
			}
			else if (countfly != 0 && GetTickCount64()-readytohighfly>200 && countfly < 4 && isGrounded) 
			{
				//(L"bbbb \n");
				SetState(GOOMBA_RED_STATE_HAS_WING_FLY_LOW);
				readytohighfly = GetTickCount64();
				countfly++;
			}
			else if (countfly == 4 && GetTickCount64()-readytohighfly>200 && isGrounded)
			{
				//DebugOut(L"cccc \n");
				SetState(GOOMBA_RED_STATE_HAS_WING_FLY_HIGH);
				countfly = 0;
			}
			else if (countfly == 0 && isGrounded)
			{
				SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
			}

		}
		else if(!haswing)
		{
			if (GetState() != GOOMBA_RED_STATE_NO_WING_DIE || GetState() != GOOMBA_RED_STATE_NO_WING_DIE_FLY)
			{
				SetState(GOOMBA_RED_STATE_NO_WING_WALK);
			}
		}
	}
	else if (id_goomba == GOOMBA_NORMAL)
	{
		if(GetState()!=GOOMBA_STATE_DIE && GetState()!=GOOMBA_STATE_DIE_FLY)
			SetState(GOOMBA_STATE_WALKING);
	}
	
	CGameObject::Update(dt);
	if(GetState()!=GOOMBA_STATE_DIE && GetState()!=GOOMBA_RED_STATE_NO_WING_DIE)
		vy += 0.0005 * dt;
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


		y += min_ty * dy + ny * 0.5f;
		x += min_tx * dx + nx * 0.5f;
		
		if (ny < 0)
			isGrounded = true;

		if (ny != 0)
			vy = 0;
	



		//Collision logic with other objects

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];
			if (id_goomba == GOOMBA_NORMAL)
			{
				if (dynamic_cast<CColorBox*>(e->obj))
				{
					if (e->nx != 0)
						x += dx;
				}
				else if (e->nx != 0)
					vx = -vx;
			}
			else if (id_goomba == GOOMBA_RED)
			{
				if (dynamic_cast<CColorBox*>(e->obj))
				{
					if (e->nx != 0)
						x += dx;
				}
				else if (e->nx != 0)
					vx = vx * -1;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{	
	if (state == GOOMBA_STATE_DIE)
	{
		if(timerenderanidie==0)
			timerenderanidie = GetTickCount64();
		if (GetTickCount64() - timerenderanidie < 300)
			animation_set->at(state)->Render(x, y);
	}
	else 
		animation_set->at(state)->Render(x, y);
	RenderBoundingBox();
}
CGoomba::CGoomba(float width, float height)
{
	this->x = width;
	this->y = height;
}
void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_DIE_FLY:
			vx = -vx;
			vy = -0.15;
			break;
		case GOOMBA_RED_STATE_NO_WING_DIE:
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_RED_STATE_NO_WING_DIE_FLY:
			vx = -vx;
			vy = -0.15;
			break;
		case GOOMBA_RED_STATE_HAS_WING_WALK:
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_RED_STATE_NO_WING_WALK:
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_RED_STATE_HAS_WING_FLY_LOW:
			vy = -0.05;
			vx = -GOOMBA_WALKING_SPEED;
			isGrounded = false;
			break;
		case GOOMBA_RED_STATE_HAS_WING_FLY_HIGH:
			isGrounded = false;
			vy = -0.15;
			vx = -GOOMBA_WALKING_SPEED;
			break;
	}
}
