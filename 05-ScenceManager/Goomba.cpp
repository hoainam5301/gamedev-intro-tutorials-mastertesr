#include "Goomba.h"
#include "ColorBox.h"
#include "MonneyEffect.h"

CGoomba::CGoomba(CMario* mario)
{
	if (id_goomba == GOOMBA_NORMAL)
		SetState(GOOMBA_STATE_WALKING);
	else if (id_goomba == GOOMBA_RED)
	{
		SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
		readyToFlyHigh = GetTickCount64();
	}
	Mario = mario;
	if (Mario->x - x > 0)
		nx = -1;
	else
		nx = 1;
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	//DebugOut(L"id gooooo %d \n", id_goomba);
	if (id_goomba == GOOMBA_NORMAL)
	{
		if (state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DIE_FLY)
		{
			left = x+4;
			top = y+8;
			right = left + GOOMBA_BBOX_WIDTH;
			bottom = top + GOOMBA_BBOX_HEIGHT+1;
		}
	}
	else if (id_goomba == GOOMBA_RED)
	{
		if (state != GOOMBA_RED_STATE_NO_WING_DIE && state != GOOMBA_RED_STATE_NO_WING_DIE_FLY)
		{
			if (hasWing)
			{
				left = x;
				top = y;
				right = x + 20;
				bottom = y + 24;
			}
			else
			{
				left = x+4;
				top = y+8;
				right = left + 16;
				bottom = top + 16;
			}
			
		}
	}
}

void CGoomba::Update(ULONGLONG dt, vector<LPGAMEOBJECT> *coObjects)
{
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 	
	//DebugOut(L"id goomba %d \n",id_goomba);
	//
	if (id_goomba == 2)
	{
		if (hasWing)
		{
			if (GetTickCount64() - sulkyMario < 5000 && sulkyMario!=0)
				SetDirection();
			if (countFly == 0 && isGrounded /*&& GetTickCount64()-readyToFlyHigh>1000*/)
			{
				
				SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
				readyToFlyHigh = GetTickCount64();
				countFly ++;
			}
			else if (countFly != 0 && GetTickCount64()-readyToFlyHigh>200 && countFly < 4 && isGrounded) 
			{
				
				SetState(GOOMBA_RED_STATE_HAS_WING_FLY_LOW);
				readyToFlyHigh = GetTickCount64();
				countFly++;
			}
			else if (countFly == 4 && GetTickCount64()-readyToFlyHigh>200 && isGrounded)
			{
			
				SetState(GOOMBA_RED_STATE_HAS_WING_FLY_HIGH);
				countFly = 0;
			}
			else if (countFly == 0 && isGrounded)
			{
				SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
			}

		}
		else 
		{
			if (GetState() != GOOMBA_RED_STATE_NO_WING_DIE && GetState() != GOOMBA_RED_STATE_NO_WING_DIE_FLY)
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
	if (makeEffect)
	{
		CMonneyEffect* monneyeffect = new CMonneyEffect();
		monneyeffect->SetPosition(x, y);
		monneyeffect->SetState(MAKE_100); 
		makeEffect = false;
		listEffect.push_back(monneyeffect);
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
				//DebugOut(L"gia tri nx %d \n ", nx);
				if (dynamic_cast<CColorBox*>(e->obj))
				{
					if (e->nx != 0)
						x += dx;
				}
				else if (e->nx != 0)
				{
					this->nx = -this->nx;
					SetState(GOOMBA_STATE_WALKING);
				}
			}
			else if (id_goomba == GOOMBA_RED)
			{
				if (dynamic_cast<CColorBox*>(e->obj))
				{
					if (e->nx != 0)
						x += dx;
				}
				else if (e->nx != 0)
				{
					this->nx = -this->nx;
					if (hasWing)
					{
						SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
						//DebugOut(L"gia tri thoi gian %d \n", GetTickCount64());	
						if (sulkyMario == 0)
							sulkyMario = GetTickCount64();
						if (GetTickCount64() - sulkyMario < 5000)
						{
							if (Mario->x - this->x > 0)
								this->nx = 1;
							else
								this->nx = -1;							
							SetState(GOOMBA_RED_STATE_HAS_WING_WALK);							
						}
						else
						{							
							sulkyMario = 0;
							SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
						}
					}
					else
						SetState(GOOMBA_STATE_WALKING);

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

void CGoomba::Render()
{	
	if (state == GOOMBA_STATE_DIE||state==GOOMBA_RED_STATE_NO_WING_DIE)
	{
		if(timeRenderAniDie==0)
			timeRenderAniDie = GetTickCount64();
		if (GetTickCount64() - timeRenderAniDie < 300)
			animation_set->at(state)->Render(x, y);
	}
	else 
		animation_set->at(state)->Render(x, y);
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Render();
	}
	//RenderBoundingBox();
}
//CGoomba::CGoomba(float width, float height)
//{
//	this->x = width;
//	this->y = height;
//}
void CGoomba::SetSpeed()
{
	if (nx > 0)
		vx = GOOMBA_WALKING_SPEED;
	if (nx < 0)
		vx = -GOOMBA_WALKING_SPEED;	
}
void CGoomba::SetDirection()
{
	if (Mario->x - x > 0)
		nx = 1;
	else
		nx = -1;
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
			SetSpeed();
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
			SetSpeed();			
			break;
		case GOOMBA_RED_STATE_NO_WING_WALK:
			SetSpeed();
			break;
		case GOOMBA_RED_STATE_HAS_WING_FLY_LOW:
			vy = -0.05;
			SetSpeed();
			isGrounded = false;
			break;
		case GOOMBA_RED_STATE_HAS_WING_FLY_HIGH:
			isGrounded = false;
			vy = -0.15;
			SetSpeed();
			break;
	}
}
