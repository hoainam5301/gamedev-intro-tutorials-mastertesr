#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopas.h"
#include "Portal.h"
#include "Items.h"
#include "Brick.h"
#include "GiantPiranhaPlant.h"
#include "Floor.h"


CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	isJumping = false;
	isSitting = false;
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	 //Simple fall down
	/*if (gravity_raccon)
	{
		DebugOut(L"Im here\n");
		vy += 0.0003 * dt;

	}
	else
	{*/
		vy += MARIO_GRAVITY * dt;
	//}

	//DebugOut(L"vy = %f\n", vy);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_ANI_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount() - timestartfly > MARIO_TIME_FLY) //het thoi gian bay thi reset lai thoi gian bat dau bay 
	{
		timestartfly = 0;
		isMaxSpeed = false;
		isFlying = false;
	}
	if (is_Grounded && vx < MARIO_RUNNING_SPEED)
		isMaxSpeed = false;

	if (isWaitingForAni && animation_set->at(state)->IsOver())
	{
		isWaitingForAni = false;
	}
	/*if (vy > 0.01)
	{
		isFalling = true;
	}*/
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		/*if (rdx != 0 && rdx!=dx)
			x += nx*abs(rdx); */
			// block every object first!
		//if (min_tx < min_ty)
		//{
		//x += min_tx * dx + nx * 0.4f;
		////CGameObject:: SweptAABBEx(coObjects);
		////LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
		//}
		//else
		//{
		//	y += min_ty * dy + ny * 0.1f;
		//	//CGameObject::SweptAABBEx(coObjects);
		//	//LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
		//}

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0)
		{
			vx = last_vx;
			if (isRunning)
				vx = 0;			
		}

		if (ny != 0)
		{
			if (ny < 0)
			{
				isJumping = false;
				is_Grounded = true;
				isFlying = false;
				Firstspaceup = true;
			}
			vy = 0;
		}		
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CColorBox*>(e->obj))
			{
				if (e->ny < 0)
				{
					isFalling = false;
				}
				if (e->nx != 0)
				{
					vx = last_vx;
					x += dx;
				}
			}
			else if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						isJumping = true;
					}
				}
				else if (e->nx != 0)
				{
					if (GetState() == MARIO_RACCON_ANI_FIGHT_IDLE_RIGHT || GetState() == MARIO_RACCON_ANI_FIGHT_IDLE_LEFT)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							goomba->SetState(GOOMBA_STATE_DIE_FLY);
							goomba->vx = -goomba->vx;
						}
					}
					else if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE || goomba->GetState()!=GOOMBA_STATE_DIE_FLY)
						{
							if (level > MARIO_LEVEL_BIG)
							{
								level = MARIO_LEVEL_BIG;
								StartUntouchable();
							}
							else if (level == MARIO_LEVEL_BIG)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
							{
								SetState(MARIO_ANI_DIE);
								return;
							}
						}
						
					}
				}
				
			} // if Goomba //aabb
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->ny < 0)
				{
					if (koopas->GetState() == KOOPAS_STATE_WALKING)
					{
						koopas->SetState(KOOPAS_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						isJumping = true;
					}
					else if (koopas->GetState() == KOOPAS_ANI_DIE_AND_MOVE)
					{
						koopas->SetState(KOOPAS_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						isJumping = true;
					}
					else if (koopas->GetState() == KOOPAS_STATE_DIE)
					{
						//DebugOut(L"nx cua mario %d \n ", lastnx);
						koopas->SetState(KOOPAS_ANI_DIE_AND_MOVE);
						if (lastnx > 0)
						{
							koopas->nx = 1;							
						}
						else						
							koopas->nx = -1;
						y += dy;
						
					}
				}
				else if (CGame::GetInstance()->IsKeyDown(DIK_A) && e->nx!=0 && koopas->GetState()==KOOPAS_STATE_DIE)// xac dinh dang nhan giu phim A va cham vs koopas 
				{
					koopas->SetState(KOOPAS_STATE_DIE_AND_IS_HOLDING);
					isHolding = true; //giu koopas van tang toc dc
					if (nx > 0)
					{
						if (level == MARIO_LEVEL_SMALL)
							SetState(MARIO_ANI_SMALL_HOLDING_TURTLE_WALK_LEFT);
						else if (level == MARIO_LEVEL_BIG)
							SetState(MARIO_ANI_BIG_HOLDING_TURTLE_WALK_LEFT);
						else if (level == MARIO_RACCON)
							SetState(MARIO_RACCON_ANI_HOLDING_TURTLE_WALK_LEFT);
						else if (level == MARIO_FIRE)
							SetState(MARIO_FIRE_ANI_HOLDING_TURTLE_WALK_LEFT);
					}
					else
					{
						if (level == MARIO_LEVEL_SMALL)
							SetState(MARIO_ANI_SMALL_HOLDING_TURTLE_WALK_RIGHT);
						else if (level == MARIO_LEVEL_BIG)
							SetState(MARIO_ANI_BIG_HOLDING_TURTLE_WALK_RIGHT);
						else if (level == MARIO_RACCON)
							SetState(MARIO_RACCON_ANI_HOLDING_TURTLE_WALK_RIGHT);
						else if (level == MARIO_FIRE)
							SetState(MARIO_FIRE_ANI_HOLDING_TURTLE_WALK_RIGHT);
						
					}

				}
				else if (koopas->GetState() == KOOPAS_STATE_DIE )
				{
					if (e->nx > 0)
					{
						//DebugOut(L"asdasdasdasd");
						if (level == MARIO_LEVEL_SMALL)
							SetState(MARIO_ANI_SMALL_KICK_LEFT);
						else if (level == MARIO_LEVEL_BIG)
							SetState(MARIO_ANI_BIG_KICK_LEFT);
						else if (level == MARIO_RACCON)
							SetState(MARIO_RACCON_ANI_KICK_LEFT);
						else if (level == MARIO_FIRE)
							SetState(MARIO_FIRE_ANI_KICK_LEFT);
						koopas->SetState(KOOPAS_ANI_DIE_AND_MOVE);
						koopas->nx = -1;
					}
					else
					{
						//DebugOut(L"huhuhuhuhuhuhu");
						if (level == MARIO_LEVEL_SMALL)
							SetState(MARIO_ANI_SMALL_KICK_RIGHT);
						else if (level == MARIO_LEVEL_BIG)
							SetState(MARIO_ANI_BIG_KICK_RIGHT);
						else if (level == MARIO_RACCON)
							SetState(MARIO_RACCON_ANI_KICK_RIGHT);
						else if (level == MARIO_FIRE)
							SetState(MARIO_FIRE_ANI_KICK_RIGHT);
						koopas->SetState(KOOPAS_ANI_DIE_AND_MOVE);
						koopas->nx = 1;
					}
				}
				else if (e->nx != 0)
				{
					if (GetState() == MARIO_RACCON_ANI_FIGHT_IDLE_RIGHT || GetState() == MARIO_RACCON_ANI_FIGHT_IDLE_LEFT)
					{
						if (koopas->GetState() != KOOPAS_STATE_DIE)
						{
							koopas->SetState(KOOPAS_STATE_DIE);
						}
					}
					else if (untouchable == 0)
					{
						if (koopas->GetState() != KOOPAS_STATE_DIE)
						{
							if (level > MARIO_LEVEL_BIG)
							{
								level = MARIO_LEVEL_BIG;
								StartUntouchable();
							}
							else if (level == MARIO_LEVEL_BIG)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
							{
								SetState(MARIO_ANI_DIE);
								return;
							}
						}
					}
					//DebugOut(L"stateeeee %d \n", koopas->GetState());
				}
				
				
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				isFalling = false;
				CBrick* Brick = dynamic_cast<CBrick*>(e->obj);
				if (e->ny > 0 && Brick->id_brick_items == ID_GACH_RA_ITEMS)
				{
					Brick->bottom_coll = 1;
					//item_animation_set = 5;
				}
				else if (e->nx != 0)
					vx = 0;
				//else if(e->ny<0 && is_grounded)
				/*else if (e->ny < 0 && this->GetState()==MARIO_STATE_JUMP)
					isJumping = false;*/
			} //if NAm
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (dynamic_cast<CFloor*>(e->obj))
			{
				if (e->ny < 0)
					isFalling = false;
				else if (e->nx != 0)
					vx = 0;
				/*else if (e->nx != 0)*/
					//DebugOut(L"Aaaaaaa");
			}
			else if (dynamic_cast<CGiantPiranhaPlant*>(e->obj))
			{
				if (untouchable == 0)
				{
					if (level > MARIO_LEVEL_BIG)
					{
						level = MARIO_LEVEL_BIG;
						StartUntouchable();
					}
					else if (level == MARIO_LEVEL_BIG)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					vx = 0;
					vy = 0;
					
				}
			}
			
		}
	}
	//DebugOut(L"isWaitingForAni = %d\n", isWaitingForAni);
	// clean up collision events
	//DebugOut(L"isfallinggggggggggggggggggg = %d\n", isFalling);
	//DebugOut(L"gia tri vx %f\n", vx);
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(state)->Render(x, y, alpha);
	//DebugOut(L"stataaaaaa %d\n", state);
	RenderBoundingBox();
}


void CMario::Collision_items(vector<LPGAMEOBJECT>* coObjects)
{
	float l, t, r, b, ln, tn, rn, btn;
	GetBoundingBox(l, t, r, b);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		e->GetBoundingBox(ln, tn, rn, btn);
		if (CGameObject::CheckAABB(l, t, r, b, ln, tn, rn, btn))
		{

			if (e->id_items == Mushroom)
			{
				e->isdone = true;
				y -= 20;
				level = MARIO_LEVEL_BIG;
			}
			else if (e->id_items == Tree_Leaf)
			{
				y -= 5;
				e->isdone = true;
				level = MARIO_RACCON;
			}
			else if (e->id_items == FIRE_FLOWER)
			{
				e->isdone = true;
				level = MARIO_FIRE;
			}
		}
	}
}

void CMario::SetState(int State)
{
	CGameObject::SetState(State);

	switch (State)
	{
	case MARIO_ANI_BIG_WALKING_RIGHT:
		
		if (isRunning)
		{
			vx += MARIO_RUNNING_ACC * dt;
			if (vx > MARIO_RUNNING_SPEED)
				vx = MARIO_RUNNING_SPEED;
			if (vx < MARIO_RUNNING_SPEED)
			{
				state = MARIO_ANI_BIG_WALKING_RIGHT;
				if (isHolding)
					state = MARIO_ANI_BIG_HOLDING_TURTLE_WALK_RIGHT;
			}
			else if (vx >= MARIO_RUNNING_SPEED)
			{
				state = MARIO_ANI_BIG_RUNNING_RIGHT;
				if (isHolding)
					state = MARIO_ANI_BIG_HOLDING_TURTLE_RUNNING_RIGHT;
			}
		}
		/*if (isHolding)
			state = MARIO_ANI_BIG_HOLDING_TURTLE_WALK_RIGHT;*/
		if (lastnx == -1 && vx <= -MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_ANI_BIG_STOP_RIGHT);
			if (isHolding)
				state = MARIO_ANI_BIG_HOLDING_TURTLE_WALK_RIGHT;
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_ANI_BIG_JUMP_RIGHT;
			else
				state = MARIO_ANI_BIG_FALLING_RIGHT;
			if (isHolding)
				state = MARIO_ANI_BIG_HOLDING_TURTLE_JUMP_RIGHT;
		}
		if (!isRunning)
		{
			vx += MARIO_WALKING_ACC * dt;
			if (vx > MARIO_WALKING_SPEED)
				vx = MARIO_WALKING_SPEED;
		}
		last_vx = vx;
		nx = 1;
		//DebugOut(L"gia tri vx=== %f \n", vx);
		break;
	case MARIO_ANI_BIG_WALKING_LEFT:
		if (isRunning)
		{
			vx -= MARIO_RUNNING_ACC * dt;
			if (vx < -MARIO_RUNNING_SPEED)
				vx = -MARIO_RUNNING_SPEED;
			if (vx > -MARIO_RUNNING_SPEED)
			{
				state = MARIO_ANI_BIG_WALKING_LEFT;
				if (isHolding)
					state = MARIO_ANI_BIG_HOLDING_TURTLE_WALK_LEFT;
			}
			else if (vx <= -MARIO_RUNNING_SPEED)
			{
				state = MARIO_ANI_BIG_RUNNING_LEFT;
				if (isHolding)
					state = MARIO_ANI_BIG_HOLDING_TURTLE_RUNNING_LEFT;
			}
		}
		/*if (isHolding)
			state = MARIO_ANI_BIG_HOLDING_TURTLE_WALK_LEFT;*/
		if (lastnx == 1 && vx >= MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_ANI_BIG_STOP_LEFT);
			if (isHolding)
				state = MARIO_ANI_BIG_HOLDING_TURTLE_WALK_LEFT;
		}
		if (isJumping )
		{
			if (vy < 0)
				state = MARIO_ANI_BIG_JUMP_LEFT;
			else
				state = MARIO_ANI_BIG_FALLING_LEFT;
			if (isHolding)
				state = MARIO_ANI_BIG_HOLDING_TURTLE_JUMP_LEFT;
		}
		if (!isRunning) 
		{
			vx -= MARIO_WALKING_ACC * dt;
			if (vx < -MARIO_WALKING_SPEED)
				vx = -MARIO_WALKING_SPEED;
		}
		last_vx = vx;
		nx = -1;
		break;
	case MARIO_ANI_SMALL_WALKING_RIGHT:
		if (isRunning)
		{
			vx += MARIO_RUNNING_ACC * dt;
			if (vx > MARIO_RUNNING_SPEED)
				vx = MARIO_RUNNING_SPEED;
			if (vx < MARIO_RUNNING_SPEED)
			{
				state = MARIO_ANI_SMALL_WALKING_RIGHT;
				if (isHolding)
					state = MARIO_ANI_SMALL_HOLDING_TURTLE_WALK_RIGHT;
			}
			else if (vx >= MARIO_RUNNING_SPEED)
			{
				state = MARIO_ANI_SMALL_RUNNING_RIGHT;
				if (isHolding)
					state = MARIO_ANI_SMALL_HOLDING_TURTLE_RUNNING_RIGHT;
			}
		}
		/*if (isHolding)
			state = MARIO_ANI_SMALL_HOLDING_TURTLE_WALK_RIGHT;*/
		if (lastnx == -1 && vx <= -MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_ANI_SMALL_STOP_LEFT);
			if (isHolding)
				state = MARIO_ANI_SMALL_HOLDING_TURTLE_WALK_RIGHT;
		}
		if (isJumping)
		{
			state = MARIO_ANI_SMALL_JUMP_RIGHT;
			if (isHolding)
				state = MARIO_ANI_SMALL_HOLDING_TURTLE_JUMP_RIGHT;
		}
		if (!isRunning) 
		{
			vx += MARIO_WALKING_ACC * dt;
			if (vx > MARIO_WALKING_SPEED)
				vx = MARIO_WALKING_SPEED;
		}
		//vx = MARIO_WALKING_SPEED;
		last_vx = vx;
		nx = 1;
		break;
	case MARIO_ANI_SMALL_WALKING_LEFT:
		if (isRunning)
		{
			vx -= MARIO_RUNNING_ACC * dt;
			if (vx < -MARIO_RUNNING_SPEED)
				vx = -MARIO_RUNNING_SPEED;
			if (vx > -MARIO_RUNNING_SPEED)
			{
				state = MARIO_ANI_SMALL_WALKING_LEFT;
				if (isHolding)
					state = MARIO_ANI_SMALL_HOLDING_TURTLE_WALK_LEFT;
			}
			else if (vx <= -MARIO_RUNNING_SPEED)
			{
				state = MARIO_ANI_SMALL_RUNNING_LEFT;
				if (isHolding)
					state = MARIO_ANI_SMALL_HOLDING_TURTLE_RUNNING_LEFT;
			}
		}
		/*if (isHolding)
			state = MARIO_ANI_SMALL_HOLDING_TURTLE_WALK_LEFT;*/
		if (lastnx == 1 && vx >= MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_ANI_SMALL_STOP_LEFT);
			if (isHolding)
				state = MARIO_ANI_SMALL_HOLDING_TURTLE_WALK_LEFT;
		}
		if (isJumping)
		{
			state = MARIO_ANI_SMALL_JUMP_LEFT;
			if (isHolding)
				state = MARIO_ANI_SMALL_HOLDING_TURTLE_JUMP_LEFT;
		}		
		if (!isRunning) {
			vx -= MARIO_WALKING_ACC * dt;
			if (vx < -MARIO_WALKING_SPEED)
				vx = -MARIO_WALKING_SPEED;
		}
		last_vx = vx;
		nx = -1;
		break;
	case MARIO_RACCON_ANI_WALK_RIGHT:
		if (isFlying)
		{
			vx += MARIO_WALKING_ACC * dt;
			if (vx > 0.75)
				vx = 0.75;
			state = MARIO_RACCON_ANI_KEEP_FLYING_RIGHT;
			if (isHolding)
				state = MARIO_RACCON_ANI_HOLDING_TURTLE_JUMP_RIGHT;
			nx = 1;
			return;
		}
		if (isRunning)
		{
			vx += MARIO_RUNNING_ACC * dt;
			if (vx > MARIO_RUNNING_SPEED)
			{
				vx = MARIO_RUNNING_SPEED;
				isMaxSpeed = true;
			}
			if (vx < MARIO_RUNNING_SPEED)
			{
				state = MARIO_RACCON_ANI_WALK_RIGHT;
				if (isHolding)
					state = MARIO_RACCON_ANI_HOLDING_TURTLE_WALK_RIGHT;
			}
			else if (vx >= MARIO_RUNNING_SPEED)
			{
				state = MARIO_RACCON_ANI_RUNNING_RIGHT;
				if (isHolding)
					state = MARIO_RACCON_ANI_HOLDING_TURTLE_RUNNING_RIGHT;
			}
		}
		/*if (isHolding)
			state = MARIO_RACCON_ANI_HOLDING_TURTLE_WALK_RIGHT;*/
		if (lastnx == -1 && vx <= -MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_RACCON_ANI_STOP_RIGHT);
			if (isHolding)
				state = MARIO_RACCON_ANI_HOLDING_TURTLE_WALK_RIGHT;
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_RACCON_ANI_JUMP_RIGHT;
			else
				state = MARIO_RACCON_ANI_FALLING_RIGHT;
			if (isHolding)
				state = MARIO_RACCON_ANI_HOLDING_TURTLE_JUMP_RIGHT;
		}
		if (!isRunning)
		{
			vx += MARIO_WALKING_ACC * dt;
			if (vx > MARIO_WALKING_SPEED)
				vx = MARIO_WALKING_SPEED;
		}
		//vx = MARIO_WALKING_SPEED;
		last_vx = vx;
		nx = 1;
		break;
	case MARIO_RACCON_ANI_WALK_LEFT:
		if (isFlying)
		{
			vx -= MARIO_WALKING_ACC * dt;
			if (vx < -0.75)
				vx = -0.75;
			state = MARIO_RACCON_ANI_KEEP_FLYING_LEFT;
			if (isHolding)  //de khi bay len thi ve animation be rua
				state = MARIO_RACCON_ANI_HOLDING_TURTLE_JUMP_LEFT;     
			nx = -1;
			return;
		}
		if (isRunning)
		{
			vx -= MARIO_RUNNING_ACC * dt;
			if (vx < -MARIO_RUNNING_SPEED)
			{
				vx = -MARIO_RUNNING_SPEED;
				isMaxSpeed = true;
			}
			if (vx > -MARIO_RUNNING_SPEED)
			{
				state = MARIO_RACCON_ANI_WALK_LEFT;
				if (isHolding)	//neu dang be rua thi ve animation be rua va di
					state = MARIO_RACCON_ANI_HOLDING_TURTLE_WALK_LEFT;
			}
			else if (vx <= -MARIO_RUNNING_SPEED)
			{
				state = MARIO_RACCON_ANI_RUNNING_LEFT;
				if (isHolding)	//neu dang be rua thi ve animation be rua va chay
					state = MARIO_RACCON_ANI_HOLDING_TURTLE_RUNNING_LEFT;
			}
		}
		/*if (isHolding)
			state = MARIO_RACCON_ANI_HOLDING_TURTLE_WALK_LEFT;*/
		if (lastnx == 1 && vx >= MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_RACCON_ANI_STOP_LEFT);
			if (isHolding)	//khi be rua thi ko ve animation quay xe
				state = MARIO_RACCON_ANI_HOLDING_TURTLE_WALK_LEFT;
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_RACCON_ANI_JUMP_LEFT;
			else
				state = MARIO_RACCON_ANI_FALLING_LEFT;
			if (isHolding)
				state = MARIO_RACCON_ANI_HOLDING_TURTLE_JUMP_LEFT;
		}
		if (!isRunning)
		{
			vx -= MARIO_WALKING_ACC * dt;
			if (vx <-MARIO_WALKING_SPEED)
				vx = -MARIO_WALKING_SPEED;
		}
		last_vx = vx;
		nx = -1;
		break;
	case MARIO_FIRE_ANI_WALK_RIGHT:
		if (isRunning)
		{
			vx += MARIO_RUNNING_ACC * dt;
			if (vx > MARIO_RUNNING_SPEED)
				vx = MARIO_RUNNING_SPEED;
			if (vx < MARIO_RUNNING_SPEED)
			{
				state = MARIO_FIRE_ANI_WALK_RIGHT;
				if (isHolding)
					state = MARIO_FIRE_ANI_HOLDING_TURTLE_WALK_RIGHT;
			}
			else if (vx >= MARIO_RUNNING_SPEED)
			{
				state = MARIO_FIRE_ANI_RUNNING_RIGHT;
				if (isHolding)
					state = MARIO_FIRE_ANI_HOLDING_TURTLE_RUNNING_RIGHT;
			}
		}
		/*if (isHolding)
			state = MARIO_FIRE_ANI_HOLDING_TURTLE_WALK_RIGHT;*/
		if (lastnx == -1 && vx <= -MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_FIRE_ANI_STOP_RIGHT);
			if (isHolding)
				state = MARIO_FIRE_ANI_HOLDING_TURTLE_WALK_RIGHT;
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_FIRE_ANI_JUMP_RIGHT;
			else
				state = MARIO_FIRE_ANI_FALLING_RIGHT;
			if (isHolding)
				state = MARIO_FIRE_ANI_HOLDING_TURTLE_JUMP_RIGHT;
		}
		if (!isRunning ) 
		{
			vx += MARIO_WALKING_ACC * dt;
			if (vx > MARIO_WALKING_SPEED)
				vx = MARIO_WALKING_SPEED;
		}
		last_vx = vx;
		nx = 1;
		break;
	case MARIO_FIRE_ANI_WALK_LEFT:
		if (isRunning)
		{
			vx -= MARIO_RUNNING_ACC * dt;
			if (vx < -MARIO_RUNNING_SPEED)
				vx = -MARIO_RUNNING_SPEED;
			if (vx > -MARIO_RUNNING_SPEED)
			{
				state = MARIO_FIRE_ANI_WALK_LEFT;
				if (isHolding)
					state = MARIO_FIRE_ANI_HOLDING_TURTLE_WALK_LEFT;
			}
			else if (vx <= -MARIO_RUNNING_SPEED)
			{
				state = MARIO_FIRE_ANI_RUNNING_LEFT;
				if (isHolding)
					state = MARIO_FIRE_ANI_HOLDING_TURTLE_RUNNING_LEFT;
			}
		}
		/*if (isHolding)
			state = MARIO_FIRE_ANI_HOLDING_TURTLE_WALK_LEFT;*/
		if (lastnx == 1 && vx >= MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_FIRE_ANI_STOP_LEFT);
			if (isHolding)
				state = MARIO_FIRE_ANI_HOLDING_TURTLE_WALK_LEFT;
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_FIRE_ANI_JUMP_LEFT;
			else
				state = MARIO_FIRE_ANI_FALLING_LEFT;
			if (isHolding)
				state = MARIO_FIRE_ANI_HOLDING_TURTLE_JUMP_LEFT;
		}
		if (!isRunning) {
			vx -= MARIO_WALKING_ACC * dt;
			if (vx < -MARIO_WALKING_SPEED)
				vx = -MARIO_WALKING_SPEED;
		}
		last_vx = vx;
		nx = -1;
		break;
	case MARIO_RACCON_ANI_JUMP_RIGHT:
		if (isSitting)
			state = MARIO_RACCON_ANI_SITTING_RIGHT;
		break;
	case MARIO_RACCON_ANI_JUMP_LEFT:
		if (isSitting)
			state = MARIO_RACCON_ANI_SITTING_LEFT;
		break;
	case MARIO_FIRE_ANI_JUMP_RIGHT:
		if (isSitting)
			state = MARIO_FIRE_ANI_SITTING_RIGHT;
		break;
	case MARIO_FIRE_ANI_JUMP_LEFT:
		if (isSitting)
			state = MARIO_FIRE_ANI_SITTING_LEFT;
		break;
	case MARIO_ANI_BIG_JUMP_RIGHT:
		if (isSitting)
			state = MARIO_ANI_BIG_SITTING_RIGHT;
		break;
	case MARIO_ANI_BIG_JUMP_LEFT:
		if (isSitting)
			state = MARIO_ANI_BIG_SITTING_LEFT;
		break;
	case MARIO_ANI_SMALL_JUMP_LEFT:
	case MARIO_ANI_SMALL_JUMP_RIGHT:
		if (isJumping)
		{
			vy = -MARIO_JUMP_SPEED_Y;
		}
		break;
	case MARIO_STATE_IDLE:
		isRunning = false;
		isSitting = false;
		if (level == MARIO_LEVEL_BIG)
		{
			if (vx > 0) {
				vx -= MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_ANI_BIG_WALKING_RIGHT;
				if (isHolding)
				{
					state = MARIO_ANI_BIG_HOLDING_TURTLE_IDLE_RIGHT;
					if (nx < 0)
						state = MARIO_ANI_BIG_HOLDING_TURTLE_IDLE_LEFT;
				}
			}
			if (vx < 0) {
				vx += MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx > 0)
					vx = 0;
				state = MARIO_ANI_BIG_WALKING_LEFT;
				if (isHolding)
				{
					state = MARIO_ANI_BIG_HOLDING_TURTLE_IDLE_LEFT;
					if(nx>0)
						state = MARIO_ANI_BIG_HOLDING_TURTLE_IDLE_RIGHT;
				}
			}
			if (nx > 0 && vx == 0)
			{
				state = MARIO_ANI_BIG_IDLE_RIGHT;
				if (isHolding)
					state = MARIO_ANI_BIG_HOLDING_TURTLE_IDLE_RIGHT;
			}
			else if (nx < 0 && vx == 0)
			{
				state = MARIO_ANI_BIG_IDLE_LEFT;
				if (isHolding)
					state = MARIO_ANI_BIG_HOLDING_TURTLE_IDLE_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (vx > 0) {
				vx -= MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_ANI_SMALL_WALKING_RIGHT;
				if (isHolding)
					state = MARIO_ANI_SMALL_HOLDING_TURTLE_IDLE_LEFT;
			}
			if (vx < 0) {
				vx += MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx > 0)
					vx = 0;
				state = MARIO_ANI_SMALL_WALKING_LEFT;
				if (isHolding)
					state = MARIO_ANI_SMALL_HOLDING_TURTLE_IDLE_RIGHT;
			}
			if (nx > 0 && vx == 0)
			{
				state = MARIO_ANI_SMALL_IDLE_RIGHT;
				if (isHolding)
					state = MARIO_ANI_SMALL_HOLDING_TURTLE_IDLE_RIGHT;
			}
			else if (nx < 0 && vx == 0)
			{
				state = MARIO_ANI_SMALL_IDLE_LEFT;
				if (isHolding)
					state = MARIO_ANI_SMALL_HOLDING_TURTLE_IDLE_LEFT;
			}
		}
		else if (level == MARIO_RACCON)
		{
			if (vx > 0) {
				vx -= MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_RACCON_ANI_WALK_RIGHT;
				if (isHolding)
				{
					state = MARIO_RACCON_ANI_HOLDING_TURTLE_IDLE_RIGHT;
					if(nx<0)
						state = MARIO_RACCON_ANI_HOLDING_TURTLE_IDLE_LEFT;
				}
			}
			if (vx < 0) {
				vx += MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx > 0)
					vx = 0;
				state = MARIO_RACCON_ANI_WALK_LEFT;
				if (isHolding)
				{
					state = MARIO_RACCON_ANI_HOLDING_TURTLE_IDLE_LEFT;
					if (nx > 0)
						state = MARIO_RACCON_ANI_HOLDING_TURTLE_IDLE_RIGHT;
				}
			}
			if (nx > 0 && vx == 0)
			{
				state = MARIO_RACCON_ANI_IDLE_RIGHT;
				if (isHolding)
					state = MARIO_RACCON_ANI_HOLDING_TURTLE_IDLE_RIGHT;
			}
			else if (nx < 0 && vx == 0)
			{
				state = MARIO_RACCON_ANI_IDLE_LEFT;
				if (isHolding)
					state = MARIO_RACCON_ANI_HOLDING_TURTLE_IDLE_LEFT;
			}
		}
		else if (level == MARIO_FIRE)
		{
			if (vx > 0) {
				vx -= MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_FIRE_ANI_WALK_RIGHT;
				if (isHolding)
					state = MARIO_FIRE_ANI_HOLDING_TURTLE_IDLE_RIGHT;
			}
			if (vx < 0) {
				vx += MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx > 0)
					vx = 0;
				state = MARIO_FIRE_ANI_WALK_LEFT;
				if (isHolding)
					state = MARIO_FIRE_ANI_HOLDING_TURTLE_IDLE_LEFT;
			}
			if (nx > 0 && vx == 0)
			{
				state = MARIO_FIRE_ANI_IDLE_RIGHT;
				if (isHolding)
					state = MARIO_FIRE_ANI_HOLDING_TURTLE_IDLE_RIGHT;
			}
			else if (nx < 0 && vx == 0)
			{
				state = MARIO_FIRE_ANI_IDLE_LEFT;
				if (isHolding)
					state = MARIO_FIRE_ANI_HOLDING_TURTLE_IDLE_LEFT;
			}
		}
		break;
	case MARIO_FIRE_ANI_FIGHT_RIGHT:
	case MARIO_FIRE_ANI_FIGHT_LEFT:
	case MARIO_RACCON_ANI_FIGHT_IDLE_RIGHT:
	case MARIO_RACCON_ANI_FIGHT_IDLE_LEFT:
	case MARIO_FIRE_ANI_FIGHT_WHILE_JUMPING_RIGHT:
	case MARIO_FIRE_ANI_FIGHT_WHILE_JUMPING_LEFT:
		ResetAni();
		isWaitingForAni = true;
		break;
	case MARIO_ANI_DIE:
		//state = MARIO_ANI_DIE;
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_ANI_BIG_SITTING_RIGHT:
	case MARIO_ANI_BIG_SITTING_LEFT:
	case MARIO_FIRE_ANI_SITTING_RIGHT:
	case MARIO_FIRE_ANI_SITTING_LEFT:
	case MARIO_RACCON_ANI_SITTING_RIGHT:
	case MARIO_RACCON_ANI_SITTING_LEFT:
		isSitting = true;
		//DebugOut(L"gia tri vx truoc khi tru %f \n", vx);
		SubRunningAcc();	
		//DebugOut(L"gia tri vx sau khi tru %f\n", vx);
		break;
	case MARIO_ANI_BIG_STOP_LEFT:
	case MARIO_ANI_BIG_STOP_RIGHT:
	case MARIO_ANI_SMALL_STOP_RIGHT:
	case MARIO_ANI_SMALL_STOP_LEFT:
	case MARIO_FIRE_ANI_STOP_RIGHT:
	case MARIO_FIRE_ANI_STOP_LEFT:
	case MARIO_RACCON_ANI_STOP_LEFT:
	case MARIO_RACCON_ANI_STOP_RIGHT:		
		SubRunningAcc();
		ResetAni();
		isWaitingForAni = true;
		break;
	case MARIO_ANI_BIG_RUNNING_RIGHT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_ANI_BIG_IDLE_RIGHT;
		break;
	case MARIO_ANI_BIG_RUNNING_LEFT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_ANI_BIG_IDLE_LEFT;
		break;
	case MARIO_ANI_SMALL_RUNNING_RIGHT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_ANI_SMALL_IDLE_RIGHT;
		break;
	case MARIO_ANI_SMALL_RUNNING_LEFT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_ANI_SMALL_IDLE_LEFT;
		break;
	case MARIO_RACCON_ANI_RUNNING_RIGHT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_RACCON_ANI_IDLE_RIGHT;
		break;
	case MARIO_RACCON_ANI_RUNNING_LEFT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_RACCON_ANI_IDLE_LEFT;
		break;
	case MARIO_FIRE_ANI_RUNNING_RIGHT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_FIRE_ANI_IDLE_RIGHT;
		break;
	case MARIO_FIRE_ANI_RUNNING_LEFT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_FIRE_ANI_IDLE_LEFT;
		
		break;
	case MARIO_RACCON_ANI_FALLING_ROCK_TAIL_RIGHT:
		if (isHolding)
			state = MARIO_RACCON_ANI_HOLDING_TURTLE_JUMP_RIGHT;
		ResetAni();
		isWaitingForAni = true;
		vy = -MARIO_GRAVITY * dt / 2;
		
		break;
	case MARIO_RACCON_ANI_FALLING_ROCK_TAIL_LEFT:	
		if (isHolding)
			state = MARIO_RACCON_ANI_HOLDING_TURTLE_JUMP_LEFT;
		ResetAni();
		isWaitingForAni = true;
		vy = -MARIO_GRAVITY*dt/2;		
		break;
	case MARIO_RACCON_ANI_KEEP_FLYING_RIGHT:
		if (isHolding)
		{
			state = MARIO_RACCON_ANI_HOLDING_TURTLE_JUMP_RIGHT;
			//DebugOut(L"im here keep fly right \n");
		}
		ResetAni();
		vx += MARIO_WALKING_ACC * dt;
		if (vx > MARIO_WALKING_SPEED)
			vx = MARIO_WALKING_SPEED;
		vy = -(MARIO_GRAVITY + 0.002f * 4) * dt;
		if (vy <= -0.1)
		{
			vy = -0.1;
		}
		break;
	case MARIO_RACCON_ANI_KEEP_FLYING_LEFT:
		if (isHolding)
		{
			state = MARIO_RACCON_ANI_HOLDING_TURTLE_JUMP_LEFT;
			//DebugOut(L"im here keep flying left \n");
		}
		ResetAni();
		vx -= MARIO_WALKING_ACC * dt;
		if (vx < -MARIO_WALKING_SPEED)
			vx = -MARIO_WALKING_SPEED;
		vy = -(MARIO_GRAVITY + 0.002f * 4) * dt;
		if (vy <= -0.1)
		{
			vy = -0.1;
		}
		
		break;
	case MARIO_RACCON_ANI_FLYING_RIGHT:
		if (isHolding)
		{
			state = MARIO_RACCON_ANI_HOLDING_TURTLE_JUMP_RIGHT;
			//DebugOut(L"im here flying right \n");
		}
		vy = -MARIO_JUMP_SPEED_Y;
		timestartfly = GetTickCount();
		break;
	case MARIO_RACCON_ANI_FLYING_LEFT:
		if (isHolding)
			state = MARIO_RACCON_ANI_HOLDING_TURTLE_JUMP_LEFT;
		vy = -MARIO_JUMP_SPEED_Y;
		timestartfly = GetTickCount();
		break;
	case MARIO_ANI_BIG_KICK_RIGHT:
	case MARIO_ANI_BIG_KICK_LEFT:
	case MARIO_ANI_SMALL_KICK_RIGHT:
	case MARIO_ANI_SMALL_KICK_LEFT:
	case MARIO_RACCON_ANI_KICK_RIGHT:
	case MARIO_RACCON_ANI_KICK_LEFT:
	case MARIO_FIRE_ANI_KICK_RIGHT:
	case MARIO_FIRE_ANI_KICK_LEFT:
		ResetAni();
		isWaitingForAni = true;
		break;
	case MARIO_ANI_BIG_HOLDING_TURTLE_WALK_RIGHT:
	case MARIO_ANI_BIG_HOLDING_TURTLE_WALK_LEFT:
		ResetAni();
		isWaitingForAni = true;
		break;
	}
	//DebugOut(L"gia tri vx %d \n", state);
}
void CMario::SubRunningAcc()
{
	if (vx > 0) {
		vx -= MARIO_SUB_RUNNING_ACC * dt;
		last_vx = vx;
		if (vx < 0)
			vx = 0;
	}
	else if (vx < 0) {
		vx += MARIO_SUB_RUNNING_ACC * dt;
		last_vx = vx;
		if (vx > 0)
			vx = 0;
	}
}
void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{


	if (level == MARIO_LEVEL_BIG)
	{
		left = x;
		top = y;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		if (nx > 0)
		{
			left = x + MARIO_BIG_BBOX_LEFT;
			right = x + MARIO_BIG_BBOX_WIDTH_RIGHT;

		}
		else
		{
			left = x;
			right = x + MARIO_BIG_BBOX_WIDTH;
		}
		if (isSitting == true)
		{
			top = y + MARIO_SIT_BBOX;
			bottom = top + MARIO_BIG_SIT_BBOX_HEIGHT;
		}
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		left = x;
		top = y;
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_RACCON)
	{
		left = x;
		top = y;
		bottom = top + MARIO_RACCON_BBOX_HEIGHT;
		right = left + MARIO_RACCON_BBOX_WIDTH;						
		if (isSitting)
		{
			top = y + MARIO_RACCON_BBOX_SIT;
			bottom = top + MARIO_RACCON_SIT_BBOX_HEIGHT;
		}
		if (nx > 0)
		{
			left = x + MARIO_SIT_BBOX;
			right = left + MARIO_RACCON_BBOX_WIDTH_RIGHT;
		}
		else
		{
			left = x + MARIO_RACCON_BBOX_LEFT;
			right = left + MARIO_RACCON_BBOX_WIDTH_RIGHT;
		}
		if ((state == MARIO_RACCON_ANI_FIGHT_IDLE_RIGHT || state == MARIO_RACCON_ANI_FIGHT_IDLE_LEFT))
		{
			if (nx > 0)
			{
				left = x + 15;
				right = left + MARIO_RACCON_BBOX_WIDTH_RIGHT;
			}
			else
			{
				left = x - 5;
				right = left + MARIO_RACCON_BBOX_WIDTH_RIGHT+10;
			}
		}
	}
	else if (level == MARIO_FIRE)
	{
		left = x;
		top = y;
		right = left + MARIO_FIRE_BBOX_WIDTH;
		bottom = top + MARIO_FIRE_BBOX_HEIGHT;
		if (isSitting)
		{
			top = y + MARIO_SIT_BBOX;
			bottom = top + MARIO_FIRE_SIT_BBOX_HEIGHT;
		}
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_RACCON);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}
