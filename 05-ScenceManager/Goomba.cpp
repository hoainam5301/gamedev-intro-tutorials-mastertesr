#include "Goomba.h"
#include "ColorBox.h"
#include "Koopas.h"
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
	if (id_goomba == GOOMBA_NORMAL)
	{
		/*if (state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DIE_FLY)
		{*/
			left = x+4;
			top = y+8;
			right = left + GOOMBA_BBOX_WIDTH;
			bottom = top + GOOMBA_BBOX_HEIGHT+1;
		//}
	}
	else if (id_goomba == GOOMBA_RED)
	{
		/*if (state != GOOMBA_RED_STATE_NO_WING_DIE && state != GOOMBA_RED_STATE_NO_WING_DIE_FLY)
		{*/
			if (hasWing)
			{
				left = x;
				top = y;
				right = left + 20;
				bottom = top + 24;
			}
			else
			{
				left = x+4;
				top = y+8;
				right = left + 16;
				bottom = top + 16;
			}
			
		//}
	}
}

void CGoomba::Update(ULONGLONG dt, vector<LPGAMEOBJECT> *coObjects)
{
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 	
	//
	/*if (y > 416)
		isdone = true;*/
	if (id_goomba == GOOMBA_RED)
	{
		if (hasWing)
		{
			if (GetTickCount64() - sulkyMario < 5000 && sulkyMario!=0)
				SetDirection();
			if (countFly == 0 && isGrounded && GetTickCount64()-readyToFlyHigh> TIME_READY_TO_FLY_HIGH)
			{
				
				SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
				readyToFlyHigh = GetTickCount64();
				countFly ++;
			}
			else if (countFly != 0 && GetTickCount64()-readyToFlyHigh> TIME_TO_FLY_LOW_AND_HIGH && countFly < 4 && isGrounded)
			{
				
				SetState(GOOMBA_RED_STATE_HAS_WING_FLY_LOW);
				readyToFlyHigh = GetTickCount64();
				countFly++;
			}
			else if (countFly == FLY_HIGH && GetTickCount64()-readyToFlyHigh> TIME_TO_FLY_LOW_AND_HIGH && isGrounded)
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
	else if (id_goomba == GOOMBA_NORMAL && !hitByTail)
	{
		if (GetState() != GOOMBA_STATE_DIE && GetState() != GOOMBA_STATE_DIE_FLY)
		{
			SetState(GOOMBA_STATE_WALKING);	
		}
	}
	if (makeEffect && !isKill)
	{
		CMonneyEffect* monneyeffect = new CMonneyEffect();
		monneyeffect->SetPosition(x, y);
		monneyeffect->SetState(MAKE_100);
		isKill = true;
		Mario->score += 100;
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


		y += min_ty * dy + ny * 0.01f;
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
				else if (dynamic_cast<CGoomba*>(e->obj))
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					if (goomba->id_goomba == GOOMBA_RED && e->ny!=0)
						y -= 1;
					else
					{
						if (id_goomba == GOOMBA_NORMAL)
						{
							this->nx = -this->nx;
							SetSpeed();
							SetState(GOOMBA_STATE_WALKING);
							if (goomba->id_goomba != GOOMBA_RED)
							{
								goomba->nx = -goomba->nx;
								goomba->SetSpeed();
								goomba->SetState(GOOMBA_STATE_WALKING);
							}
						}
					}
				}
				else if (e->nx != 0 && !dynamic_cast<CKoopas*>(e->obj) && !hitByTail && !hitByWeapon)
				{
					this->nx = -this->nx;
					SetSpeed();
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
				else if (e->nx != 0 /*&& !dynamic_cast<CKoopas*>(e->obj)*/)
				{
					this->nx = -this->nx;
					//SetSpeed();
					if (hasWing)
					{
						SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
						//DebugOut(L"gia tri thoi gian %d \n", GetTickCount64());	
						if (sulkyMario == 0)
							sulkyMario = GetTickCount64();
						if (GetTickCount64() - sulkyMario < TIME_READY_TO_SULKY)
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
					{
						if(!hitByWeapon)
							SetState(GOOMBA_RED_STATE_NO_WING_WALK);
					}
				}
			}
		}
	}
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Update(dt, coObjects);
		if (listEffect[i]->isdone)
		{
			if(id_goomba==GOOMBA_NORMAL)
				isdone = true;
			/*else if (id_goomba == GOOMBA_RED)
			{
				if (!hasWing && (state==GOOMBA_RED_STATE_NO_WING_DIE || state==GOOMBA_RED_STATE_NO_WING_DIE_FLY) )
					isdone = true;
			}*/
		}
	}
	//if(id_goomba==GOOMBA_NORMAL)
	//DebugOut(L"gia tri state %d \n", this->nx);
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{	
	DebugOut(L"gia tri state %d \n", state);
	if (state == GOOMBA_STATE_DIE||state==GOOMBA_RED_STATE_NO_WING_DIE||state==GOOMBA_STATE_DIE_FLY||state==GOOMBA_RED_STATE_NO_WING_DIE_FLY)
	{
		if(timeRenderAniDie==0)
			timeRenderAniDie = GetTickCount64();
		if (GetTickCount64() - timeRenderAniDie < TIME_RENDER_ANI_DIE)
			animation_set->at(state)->Render(x, y);
		else
			isdone = true;
	}
	else 
		animation_set->at(state)->Render(x, y);
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Render();
	}
	//RenderBoundingBox();
}

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
			if (hitByTail)
			{
				SetDirection();
				SetSpeed();
			}
			else		
				vx = -vx;
			vy = -(GOOMBA_FLY_HIGH_SPEED);
			break;
		case GOOMBA_RED_STATE_NO_WING_DIE:
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_RED_STATE_NO_WING_DIE_FLY:
			vx = -vx;
			vy = -GOOMBA_FLY_HIGH_SPEED;
			break;
		case GOOMBA_RED_STATE_HAS_WING_WALK:
			SetSpeed();			
			break;
		case GOOMBA_RED_STATE_NO_WING_WALK:
			SetSpeed();
			break;
		case GOOMBA_RED_STATE_HAS_WING_FLY_LOW:
			vy = -GOOMBA_FLY_LOW_SPEED;
			SetSpeed();
			isGrounded = false;
			break;
		case GOOMBA_RED_STATE_HAS_WING_FLY_HIGH:
			isGrounded = false;
			vy = -GOOMBA_FLY_HIGH_SPEED;
			SetSpeed();
			break;
	}
}
