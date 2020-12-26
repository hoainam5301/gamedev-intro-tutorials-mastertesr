#include <algorithm>
#include <assert.h>
//#include "Utils.h"

#include "Mario.h"
//#include "Game.h"

#include "Goomba.h"
#include "Koopas.h"
#include "Portal.h"
#include "Items.h"
#include "Brick.h"
#include "GiantPiranhaPlant.h"
#include "Floor.h"
#include "ColorBox.h"
#include "BrokenBrick.h"
#include "Pipe.h"
#include "Coin.h"
#include "GiantPiranhaPlantBite.h"


CMario* CMario::__instance = NULL;

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

void CMario::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects,vector <LPGAMEOBJECT>* listObjIdle)
{
#pragma region Update Mario
	// Calculate dx, dy 
	CGameObject::Update(dt);

	 //Simple fall down
	if (!getDownInPipe && !getUpInPipe)
		vy += MARIO_GRAVITY * dt;
	else
		y += dy;
	
	if (getDownInPipe)
	{
		if (topOfMario < startOfLongPipeY)
		{
			SetState(MARIO_RACCOON_STATE_MOVE_IN_PIPE);

		}
		else if (topOfMario >= startOfLongPipeY)
		{
			turnOffLight = true;
			inHiddenArea = true;
			if (waitGetOutOfPipe == 0)
				waitGetOutOfPipe = GetTickCount64();
			if (GetTickCount64() - waitGetOutOfPipe >= MARIO_TIME_GO_IN_PIPE /*&& waitGetOutOfPipe != 0*/)
			{
				getDownInPipe = false;
				SetPosition(2100, 479);
				turnOffLight = false;
				waitGetOutOfPipe = 0;
				countRender = 0;
			}
		}
	}	
	if (getUpInPipe)
	{

		if (topOfMario > atEndOfPipeY && atEndOfPipeY != 0)
		{
			vy = -0.01;
			SetState(MARIO_RACCOON_STATE_MOVE_IN_PIPE);
			waitGetOutOfPipe = GetTickCount64();
		}
		else if (topOfMario <= atEndOfPipeY)
		{
			inHiddenArea = false;
			turnOffLight = true;
			countRender = 0;
			vy = 0;
			if (GetTickCount64() - waitGetOutOfPipe >= MARIO_TIME_GO_IN_PIPE)
			{
				SetPosition(2320, 384);
				atEndOfPipeY = 0;//khong bi phai set vi tri lien tuc nhieu lan
				turnOffLight = false;
				waitGetOutOfPipe = 0;
			}
		}
		else if (topOfMario > topOfPipeOut)
		{
			vy = -0.01;
			SetState(MARIO_RACCOON_STATE_MOVE_IN_PIPE);
		}
		else if (topOfMario <= topOfPipeOut)
		{
			getUpInPipe = false;
			SetState(MARIO_STATE_IDLE);
		}
	}

	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	if (GetTickCount64() - timestartfly > MARIO_TIME_FLY) //het thoi gian bay thi reset lai thoi gian bat dau bay 
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

#pragma endregion

#pragma region	ObjMove
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (state != MARIO_ANI_DIE)
		CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		mDy += dy;
		if (mDy > 0.5)
		{
			isJumping = true;
			is_Grounded = false;	
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	
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
				canNotWalking = false;	
				standOnPipe = false;
				mDy = 0;				
			}
			vy = 0;
		}
		//
		// Collision logic with other objects
		//
		Collision_coin(coObjects);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			/*if (dynamic_cast<CColorBox*>(e->obj))
			{
				if (e->ny < 0)
				{
					isFalling = false;
					inHighArea = false;
				}
				if (e->nx != 0)
				{
					if (!isFlying)					
						vx = last_vx;
					x += dx;					
				}
			}
			else*/ if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->id_goomba == GOOMBA_NORMAL)     //kill goomba normal
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							goomba->makeEffect = true;
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							isJumping = true;
						}
					}
					else if (goomba->id_goomba == GOOMBA_RED)				//kill goomba red
					{
						if (goomba->hasWing)
						{
							goomba->SetState(GOOMBA_RED_STATE_NO_WING_WALK);	//khi co canh thi ve trang thai di bo							
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							goomba->hasWing = false;
							isJumping = true;
						}
						else
						{
							if (goomba->GetState() != GOOMBA_RED_STATE_NO_WING_DIE)
							{
								goomba->SetState(GOOMBA_RED_STATE_NO_WING_DIE);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
								isJumping = true;
							}
						}
					}
				}
				else if (e->ny > 0)
				{
					y -= dy*2;
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->id_goomba == GOOMBA_NORMAL)
						{
							if (goomba->GetState() != GOOMBA_STATE_DIE || goomba->GetState() != GOOMBA_STATE_DIE_FLY)
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
						else if (goomba->id_goomba == GOOMBA_RED)
						{
							if (goomba->GetState() != GOOMBA_RED_STATE_NO_WING_DIE || goomba->GetState() != GOOMBA_RED_STATE_NO_WING_DIE_FLY)
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
				}
				
			} // if Goomba //aabb
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (koopas->id_koopa == KOOPA_RED)
				{
					if (e->ny < 0)
					{
						if (koopas->GetState() == KOOPA_RED_STATE_WALKING_RIGHT || koopas->GetState() == KOOPA_RED_STATE_WALKING_LEFT)
						{
							koopas->SetState(KOOPA_RED_STATE_DIE);
							koopas->makeEffect = true;
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							isJumping = true;
						}
						else if (koopas->GetState() == KOOPA_RED_STATE_DIE_AND_MOVE)
						{
							koopas->SetState(KOOPA_RED_STATE_DIE);
							koopas->makeEffect = true;
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							isJumping = true;
						}
						else if (koopas->GetState() == KOOPA_RED_STATE_DIE_AND_MOVE_UP)
						{
							koopas->SetState(KOOPA_RED_STATE_DIE_UP);
							koopas->makeEffect = true;
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							isJumping = true;
						}
						else if (koopas->GetState() == KOOPA_RED_STATE_DIE)
						{

							koopas->SetState(KOOPA_RED_STATE_DIE_AND_MOVE);
							koopas->makeEffect = true;
							if (lastnx > 0)
							{
								koopas->nx = 1;
							}
							else
								koopas->nx = -1;
							y += dy;

						}
						else if (koopas->GetState() == KOOPA_RED_STATE_DIE_UP)
						{
							koopas->SetState(KOOPA_RED_STATE_DIE_AND_MOVE_UP);
							koopas->makeEffect = true;
							if (lastnx > 0)
							{
								koopas->nx = 1;
							}
							else
								koopas->nx = -1;
							y += dy;
						}
					}
					else if (CGame::GetInstance()->IsKeyDown(DIK_A) && e->nx != 0 && (koopas->GetState() == KOOPA_RED_STATE_DIE || koopas->GetState() == KOOPA_RED_STATE_DIE_UP))// xac dinh dang nhan giu phim A va cham vs koopas 
					{
						if (koopas->last_state == KOOPA_RED_STATE_DIE)
							koopas->SetState(KOOPA_RED_STATE_HOLDING);
						else if (koopas->last_state == KOOPA_RED_STATE_DIE_UP)
							koopas->SetState(KOOPA_RED_STATE_HOLDING_UP);
						isHolding = true; //giu koopas van tang toc dc
						if (nx > 0)
						{
							if (level == MARIO_LEVEL_SMALL)
								SetState(MARIO_SMALL_STATE_HOLDING_TURTLE_WALK_LEFT);
							else if (level == MARIO_LEVEL_BIG)
								SetState(MARIO_BIG_STATE_HOLDING_TURTLE_WALK_LEFT);
							else if (level == MARIO_RACCOON)
								SetState(MARIO_RACCOON_STATE_HOLDING_TURTLE_WALK_LEFT);
							else if (level == MARIO_FIRE)
								SetState(MARIO_FIRE_STATE_HOLDING_TURTLE_WALK_LEFT);
						}
						else
						{
							if (level == MARIO_LEVEL_SMALL)
								SetState(MARIO_SMALL_STATE_HOLDING_TURTLE_WALK_RIGHT);
							else if (level == MARIO_LEVEL_BIG)
								SetState(MARIO_BIG_STATE_HOLDING_TURTLE_WALK_RIGHT);
							else if (level == MARIO_RACCOON)
								SetState(MARIO_RACCOON_STATE_HOLDING_TURTLE_WALK_RIGHT);
							else if (level == MARIO_FIRE)
								SetState(MARIO_FIRE_STATE_HOLDING_TURTLE_WALK_RIGHT);
						}

					}
					else if (koopas->GetState() == KOOPA_RED_STATE_DIE || koopas->GetState() == KOOPA_RED_STATE_DIE_UP)
					{
						if (e->nx > 0)
						{
							if (level == MARIO_LEVEL_SMALL)
								SetState(MARIO_SMALL_STATE_KICK_LEFT);
							else if (level == MARIO_LEVEL_BIG)
								SetState(MARIO_BIG_STATE_KICK_LEFT);
							else if (level == MARIO_RACCOON)
								SetState(MARIO_RACCOON_STATE_KICK_LEFT);
							else if (level == MARIO_FIRE)
								SetState(MARIO_FIRE_STATE_KICK_LEFT);

							koopas->nx = -1;
							if (koopas->last_state == KOOPA_RED_STATE_DIE)
								koopas->SetState(KOOPA_RED_STATE_DIE_AND_MOVE);
							else if (koopas->last_state == KOOPA_RED_STATE_DIE_UP)
								koopas->SetState(KOOPA_RED_STATE_DIE_AND_MOVE_UP);
						}
						else
						{
							if (level == MARIO_LEVEL_SMALL)
								SetState(MARIO_SMALL_STATE_KICK_RIGHT);
							else if (level == MARIO_LEVEL_BIG)
								SetState(MARIO_BIG_STATE_KICK_RIGHT);
							else if (level == MARIO_RACCOON)
								SetState(MARIO_RACCOON_STATE_KICK_RIGHT);
							else if (level == MARIO_FIRE)
								SetState(MARIO_FIRE_STATE_KICK_RIGHT);

							koopas->nx = 1;
							if (koopas->last_state == KOOPA_RED_STATE_DIE)
								koopas->SetState(KOOPA_RED_STATE_DIE_AND_MOVE);
							else if (koopas->last_state == KOOPA_RED_STATE_DIE_UP)
								koopas->SetState(KOOPA_RED_STATE_DIE_AND_MOVE_UP);
						}
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (koopas->GetState() != KOOPA_RED_STATE_DIE && koopas->GetState() != KOOPA_RED_STATE_DIE_UP && !isHolding)
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
				}
				else if (koopas->id_koopa == KOOPA_GREEN)
				{					
					if (e->ny < 0)
					{
						if (koopas->GetState() == KOOPA_GREEN_STATE_HAS_WING_FLY_RIGHT || koopas->GetState() == KOOPA_GREEN_STATE_HAS_WING_FLY_LEFT)
						{
							if (koopas->nx > 0)
								koopas->SetState(KOOPA_GREEN_STATE_WALKING_RIGHT);
							else
								koopas->SetState(KOOPA_GREEN_STATE_WALKING_LEFT);
							koopas->makeEffect = true;
							koopas->hasWing = false;
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							isJumping = true;
						}
						else if (koopas->GetState() == KOOPA_GREEN_STATE_WALKING_LEFT || koopas->GetState() == KOOPA_GREEN_STATE_WALKING_RIGHT)
						{
							koopas->SetState(KOOPA_GREEN_STATE_DIE);
							koopas->makeEffect = true;
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							isJumping = true;
						}
						else if (koopas->GetState() == KOOPA_GREEN_STATE_DIE_AND_MOVE)
						{
							koopas->SetState(KOOPA_GREEN_STATE_DIE);
							koopas->makeEffect = true;
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							isJumping = true;
						}
						else if (koopas->GetState() == KOOPA_GREEN_STATE_DIE_AND_MOVE_UP)
						{
							koopas->SetState(KOOPA_GREEN_STATE_DIE_UP);
							koopas->makeEffect = true;
							vy = -MARIO_JUMP_DEFLECT_SPEED;
							isJumping = true;
						}
						else if (koopas->GetState() == KOOPA_GREEN_STATE_DIE)
						{

							koopas->SetState(KOOPA_GREEN_STATE_DIE_AND_MOVE);
							koopas->makeEffect = true;
							if (lastnx > 0)
							{
								koopas->nx = 1;
							}
							else
								koopas->nx = -1;
							y += dy;

						}
						else if (koopas->GetState() == KOOPA_GREEN_STATE_DIE_UP)
						{
							koopas->SetState(KOOPA_GREEN_STATE_DIE_AND_MOVE_UP);
							koopas->makeEffect = true;
							if (lastnx > 0)
							{
								koopas->nx = 1;
							}
							else
								koopas->nx = -1;
							y += dy;
						}
					}
					else if (CGame::GetInstance()->IsKeyDown(DIK_A) && e->nx != 0 && (koopas->GetState() == KOOPA_GREEN_STATE_DIE || koopas->GetState() == KOOPA_GREEN_STATE_DIE_UP))// xac dinh dang nhan giu phim A va cham vs koopas 
					{
						if (koopas->last_state == KOOPA_GREEN_STATE_DIE)
							koopas->SetState(KOOPA_GREEN_STATE_HOLDING);
						else if (koopas->last_state == KOOPA_GREEN_STATE_DIE_UP)
							koopas->SetState(KOOPA_GREEN_STATE_HOLDING_UP);
						isHolding = true; //giu koopas van tang toc dc
						if (nx > 0)
						{
							if (level == MARIO_LEVEL_SMALL)
								SetState(MARIO_SMALL_STATE_HOLDING_TURTLE_WALK_LEFT);
							else if (level == MARIO_LEVEL_BIG)
								SetState(MARIO_BIG_STATE_HOLDING_TURTLE_WALK_LEFT);
							else if (level == MARIO_RACCOON)
								SetState(MARIO_RACCOON_STATE_HOLDING_TURTLE_WALK_LEFT);
							else if (level == MARIO_FIRE)
								SetState(MARIO_FIRE_STATE_HOLDING_TURTLE_WALK_LEFT);
						}
						else
						{
							if (level == MARIO_LEVEL_SMALL)
								SetState(MARIO_SMALL_STATE_HOLDING_TURTLE_WALK_RIGHT);
							else if (level == MARIO_LEVEL_BIG)
								SetState(MARIO_BIG_STATE_HOLDING_TURTLE_WALK_RIGHT);
							else if (level == MARIO_RACCOON)
								SetState(MARIO_RACCOON_STATE_HOLDING_TURTLE_WALK_RIGHT);
							else if (level == MARIO_FIRE)
								SetState(MARIO_FIRE_STATE_HOLDING_TURTLE_WALK_RIGHT);
						}

					}
					else if (koopas->GetState() == KOOPA_GREEN_STATE_DIE || koopas->GetState() == KOOPA_GREEN_STATE_DIE_UP)
					{
						if (e->nx > 0)
						{
							if (level == MARIO_LEVEL_SMALL)
								SetState(MARIO_SMALL_STATE_KICK_LEFT);
							else if (level == MARIO_LEVEL_BIG)
								SetState(MARIO_BIG_STATE_KICK_LEFT);
							else if (level == MARIO_RACCOON)
								SetState(MARIO_RACCOON_STATE_KICK_LEFT);
							else if (level == MARIO_FIRE)
								SetState(MARIO_FIRE_STATE_KICK_LEFT);

							koopas->nx = -1;
							if (koopas->last_state == KOOPA_GREEN_STATE_DIE)
								koopas->SetState(KOOPA_GREEN_STATE_DIE_AND_MOVE);
							else if (koopas->last_state == KOOPA_GREEN_STATE_DIE_UP)
								koopas->SetState(KOOPA_GREEN_STATE_DIE_AND_MOVE_UP);
						}
						else
						{
							if (level == MARIO_LEVEL_SMALL)
								SetState(MARIO_SMALL_STATE_KICK_RIGHT);
							else if (level == MARIO_LEVEL_BIG)
								SetState(MARIO_BIG_STATE_KICK_RIGHT);
							else if (level == MARIO_RACCOON)
								SetState(MARIO_RACCOON_STATE_KICK_RIGHT);
							else if (level == MARIO_FIRE)
								SetState(MARIO_FIRE_STATE_KICK_RIGHT);

							koopas->nx = 1;
							if (koopas->last_state == KOOPA_GREEN_STATE_DIE)
								koopas->SetState(KOOPA_GREEN_STATE_DIE_AND_MOVE);
							else if (koopas->last_state == KOOPA_GREEN_STATE_DIE_UP)
								koopas->SetState(KOOPA_GREEN_STATE_DIE_AND_MOVE_UP);
						}
					}
					else if (e->nx != 0)
					{
						if (untouchable == 0)
						{
							if (koopas->GetState() != KOOPA_GREEN_STATE_DIE && koopas->GetState() != KOOPA_GREEN_STATE_DIE_UP && !isHolding)
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
				}

			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{				
			CBrick* Brick = dynamic_cast<CBrick*>(e->obj);
			
				if (e->ny > 0 )
				{
					Brick->bottom_coll = 1;
				//	DebugOut(L"NNNNNNNNNNNNNNNNNNNNNNNNNNNNN\n");
				}
				else if (e->ny < 0)
				{
					isFalling = false;	
					inHighArea = false;
				}
				else if (e->nx != 0)
					vx = 0;
			} //if NAm		
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
			else if (dynamic_cast<CGiantPiranhaPlantBite*>(e->obj))
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
		/*	else if (dynamic_cast<CFloor*>(e->obj))
			{
				if (e->ny < 0)
				{
					isFalling = false;
					inHighArea = false;
				}
				else if (e->nx != 0)
					vx = 0;
			}
			else if (dynamic_cast<CPipe*>(e->obj))
			{
				CPipe* pipe = dynamic_cast<CPipe*>(e->obj);
				if (e->ny < 0 && CGame::GetInstance()->IsKeyDown(DIK_DOWN))
				{
					if (pipe->pipeType == PIPE_WITH_LONG_ANI)
					{
						startOfLongPipeY = pipe->y;
						getDownInPipe = true;
						vy = MARIO_MOVING_IN_PIPE_SPEED;
						isSitting = false;
						SetState(MARIO_RACCOON_STATE_MOVE_IN_PIPE);
					}
				}
				else if (e->ny < 0)
				{
					isFalling = false;
					inHighArea = false;
					standOnPipe = true;
				}
			}*/
			else if (dynamic_cast<CBrokenBrick*>(e->obj))
			{
				CBrokenBrick* brokenbrick = dynamic_cast<CBrokenBrick*>(e->obj);	
				if (brokenbrick->GetState() == STATE_COIN_ROTATE || brokenbrick->GetState() == STATE_COIN_NO_ROTATE)
				{
					brokenbrick->isDestroyed = true;
					brokenbrick->isdone = true;				
					//y += dy;
				}
				else if (e->ny < 0 && (brokenbrick->GetState() == STATE_COIN_ROTATE || brokenbrick->GetState()==STATE_COIN_NO_ROTATE))
				{
					brokenbrick->isDestroyed = true;
					brokenbrick->isdone = true;
					x += dx;
					y += dy;
				}
				else if (e->ny < 0 && brokenbrick->GetState()==STATE_BRICK_NORMAL)
				{
					isFalling = false;	
					inHighArea = false;
				}
				/*else if(brokenbrick->GetState() != STATE_BRICK_NORMAL)
				{
					brokenbrick->isDestroyed = true;
					brokenbrick->isdone = true;
				}*/				
			}
		}
		
	}
	// clean up collision events
	//DebugOut(L"gia tri  %f \n", vx);
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
#pragma endregion

#pragma region Collision IdleObj
	vector<LPCOLLISIONEVENT> coObjIdlEvents;
	vector<LPCOLLISIONEVENT> coObjIdleEventsResult;

	coObjIdlEvents.clear();	
	CalcPotentialCollisions(listObjIdle, coObjIdlEvents);

	if (coObjIdlEvents.size() == 0)
	{
		x += dx;
		y += dy;
		mDy += dy;
		if (mDy > 0.5)
		{
			isJumping = true;
			is_Grounded = false;
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coObjIdlEvents, coObjIdleEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

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
				canNotWalking = false;
				standOnPipe = false;
				mDy = 0;
			}
			vy = 0;
		}
		//
		// Collision logic with other objects
		//
		Collision_coin(coObjects);
		for (UINT i = 0; i < coObjIdleEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coObjIdleEventsResult[i];
			if (dynamic_cast<CColorBox*>(e->obj))
			{
				if (e->ny < 0)
				{
					isFalling = false;
					inHighArea = false;
				}
				if (e->nx != 0)
				{
					if(!isFlying)
						vx = last_vx;
					x += dx;
				}
			} 
			else if (dynamic_cast<CFloor*>(e->obj))
			{
				if (e->ny < 0)
				{
					isFalling = false;
					inHighArea = false;
				}
				else if (e->nx != 0)
					vx = 0;
				//DebugOut(L"gia tri  %f \n", vy);
			}
			else if (dynamic_cast<CPipe*>(e->obj))
			{
				CPipe* pipe = dynamic_cast<CPipe*>(e->obj);
				if (e->ny < 0 && CGame::GetInstance()->IsKeyDown(DIK_DOWN))
				{
					if (pipe->pipeType == 2)
					{
						startOfLongPipeY = pipe->y;
						getDownInPipe = true;
						vy = 0.01;
						isSitting = false;
						SetState(MARIO_RACCOON_STATE_MOVE_IN_PIPE);
					}
				}
				else if (e->ny < 0)
				{
					isFalling = false;
					inHighArea = false;
					standOnPipe = true;
				}
			}
		}

	}
	// clean up collision events
	//DebugOut(L"gia tri  %f \n", vx);
	for (UINT i = 0; i < coObjIdlEvents.size(); i++) delete coObjIdlEvents[i];

#pragma endregion
	
}

void CMario::UpdateInScenceMap(ULONGLONG dt)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
	if (vx > 0)
	{
		/*if (leftOfMario > posXOfNextPortalGoRight)
		{
			vx = 0;
			leftOfMario = posXOfNextPortalGoRight;
			x = leftOfMario - 12;
			isAutoGoRight = false;
			isGoTo = false;
			sceneIdPresent = sceneIdGoToRight;
		}*/
		if (x > posXOfNextPortalGoRight)
		{
			vx = 0;
			x = posXOfNextPortalGoRight- MARIO_BONUS_POS_X;
			//x = leftOfMario - 12;
			isAutoGoRight = false;
			isGoTo = false;
			sceneIdPresent = sceneIdGoToRight;
		}
	}
	else if (vx < 0)
	{
		/*if (leftOfMario < posXOfNextPortalGoLeft)
		{
			vx = 0;
			leftOfMario = posXOfNextPortalGoLeft;
			x = leftOfMario - 12;
			isAutoGoLeft = false;
			isGoTo = false;
			sceneIdPresent = sceneIdGoToLeft;
		}*/
		if (x < posXOfNextPortalGoLeft)
		{
			vx = 0;
			//leftOfMario = posXOfNextPortalGoLeft;
			x = posXOfNextPortalGoLeft - MARIO_BONUS_POS_X;
			isAutoGoLeft = false;
			isGoTo = false;
			sceneIdPresent = sceneIdGoToLeft;
		}
	}
	else if (vy > 0)
	{
		/*if (topOfMario > posYOfNextPortalGoDown)
		{
			vy = 0;
			topOfMario = posYOfNextPortalGoDown;
			y = topOfMario - 14;
			isAutoGoDown = false;
			isGoTo = false;
			leftOfMario = posXOfPortal;
			x = leftOfMario - 12;
			sceneIdPresent = sceneIdGoToDown;
		}*/
		if (y > posYOfNextPortalGoDown)
		{
			vy = 0;
			y = posYOfNextPortalGoDown- MARIO_BONUS_POS_Y;
			//y = topOfMario - 14;
			isAutoGoDown = false;
			isGoTo = false;
			x = posXOfPortal- MARIO_BONUS_POS_X;
			//x = leftOfMario - 12;
			sceneIdPresent = sceneIdGoToDown;
		}
	}
	else if (vy < 0)
	{
		/*if (topOfMario < posYOfNextPortalGoUp)
		{
			vy = 0;
			topOfMario = posYOfNextPortalGoUp;
			y = topOfMario - 14;
			isAutoGoTop = false;
			isGoTo = false;
			leftOfMario = posXOfPortal;	
			x = leftOfMario - 12;
			sceneIdPresent = sceneIdGoToUp;
		}*/
		if (y < posYOfNextPortalGoUp)
		{
			vy = 0;
			y = posYOfNextPortalGoUp- MARIO_BONUS_POS_Y;
			//y = topOfMario - 14;
			isAutoGoTop = false;
			isGoTo = false;
			x = posXOfPortal- MARIO_BONUS_POS_X;
			//x = leftOfMario - 12;
			sceneIdPresent = sceneIdGoToUp;
		}
	}
	

}

void CMario::Render()
{
	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(state)->Render(x, y, alpha);	
	//RenderBoundingBox();
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
				e->makeEffect = true;				
				level = MARIO_LEVEL_BIG;
			}
			else if (e->id_items == Tree_Leaf)
			{				
				e->isdone = true;
				e->makeEffect = true;
				level = MARIO_RACCOON;
			}
			else if (e->id_items == FIRE_FLOWER)
			{
				e->isdone = true;
				e->makeEffect=true;
				level = MARIO_FIRE;
			}
			else if (e->id_items == SWITCH_P_ON)
			{
				e->SetState(SWITCH_P_OFF);
			}
			else if (e->id_items == ITEMS_END_GAME)
			{
				CItems* items = dynamic_cast<CItems*>(e);
				items->makeItemFly = true;
				e->isdone = true;
			}
		}
	}
}

void CMario::Collision_coin(vector<LPGAMEOBJECT>* coObjects)
{
	float l, t, r, b, ln, tn, rn, btn;
	GetBoundingBox(l, t, r, b);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		e->GetBoundingBox(ln, tn, rn, btn);
		if (CGameObject::CheckAABB(l, t, r, b, ln, tn, rn, btn))
		{
			if (dynamic_cast<CCoin*>(e))
			{
				CCoin* brokenbrick = dynamic_cast<CCoin*>(e);
				brokenbrick->GetBoundingBox(ln, tn, rn, btn);
				if (CGameObject::CheckAABB(l, t, r, b, ln, tn, rn, btn))
				{
					//if (brokenbrick->GetState() == STATE_COIN_ROTATE || brokenbrick->GetState() == STATE_COIN_NO_ROTATE)
						brokenbrick->isdone=true;
						dola++;
						score += 100;
				}
			}
			
		}
	}
}

void CMario::SetState(int State)
{
	CGameObject::SetState(State);

	switch (State)
	{
	case MARIO_BIG_STATE_WALKING_RIGHT:
		
		if (isRunning)
		{
			vx += MARIO_RUNNING_ACC * dt;
			if (vx > MARIO_RUNNING_SPEED)
				vx = MARIO_RUNNING_SPEED;
			if (vx < MARIO_RUNNING_SPEED)
			{
				state = MARIO_BIG_STATE_WALKING_RIGHT;
				if (isHolding)
					state = MARIO_BIG_STATE_HOLDING_TURTLE_WALK_RIGHT;
			}
			else if (vx >= MARIO_RUNNING_SPEED)
			{
				state = MARIO_BIG_STATE_RUNNING_RIGHT;
				if (isHolding)
					state = MARIO_BIG_STATE_HOLDING_TURTLE_RUNNING_RIGHT;
			}
		}
		/*if (isHolding)
			state = MARIO_BIG_STATE_HOLDING_TURTLE_WALK_RIGHT;*/
		if (lastnx == -1 && vx <= -MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_BIG_STATE_STOP_RIGHT);
			if (isHolding)
				state = MARIO_BIG_STATE_HOLDING_TURTLE_WALK_RIGHT;
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_BIG_STATE_JUMP_RIGHT;
			else
				state = MARIO_BIG_STATE_FALLING_RIGHT;
			if (isHolding)
				state = MARIO_BIG_STATE_HOLDING_TURTLE_JUMP_RIGHT;
		}
		if (!isRunning)
		{
			vx += MARIO_WALKING_ACC *dt;
			if (vx > MARIO_WALKING_SPEED)
				vx = MARIO_WALKING_SPEED;
		}
		last_vx = vx;
		nx = 1;
		//DebugOut(L"gia tri vx=== %f \n", vx);
		break;
	case MARIO_BIG_STATE_WALKING_LEFT:
		if (isRunning)
		{
			vx -= MARIO_RUNNING_ACC ;
			if (vx < -MARIO_RUNNING_SPEED)
				vx = -MARIO_RUNNING_SPEED;
			if (vx > -MARIO_RUNNING_SPEED)
			{
				state = MARIO_BIG_STATE_WALKING_LEFT;
				if (isHolding)
					state = MARIO_BIG_STATE_HOLDING_TURTLE_WALK_LEFT;
			}
			else if (vx <= -MARIO_RUNNING_SPEED)
			{
				state = MARIO_BIG_STATE_RUNNING_LEFT;
				if (isHolding)
					state = MARIO_BIG_STATE_HOLDING_TURTLE_RUNNING_LEFT;
			}
		}
		/*if (isHolding)
			state = MARIO_BIG_STATE_HOLDING_TURTLE_WALK_LEFT;*/
		if (lastnx == 1 && vx >= MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_BIG_STATE_STOP_LEFT);
			if (isHolding)
				state = MARIO_BIG_STATE_HOLDING_TURTLE_WALK_LEFT;
		}
		if (isJumping )
		{
			if (vy < 0)
				state = MARIO_BIG_STATE_JUMP_LEFT;
			else
				state = MARIO_BIG_STATE_FALLING_LEFT;
			if (isHolding)
				state = MARIO_BIG_STATE_HOLDING_TURTLE_JUMP_LEFT;
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
	case MARIO_SMALL_STATE_WALKING_RIGHT:
		if (isRunning)
		{
			vx += MARIO_RUNNING_ACC * dt;
			if (vx > MARIO_RUNNING_SPEED)
				vx = MARIO_RUNNING_SPEED;
			if (vx < MARIO_RUNNING_SPEED)
			{
				state = MARIO_SMALL_STATE_WALKING_RIGHT;
				if (isHolding)
					state = MARIO_SMALL_STATE_HOLDING_TURTLE_WALK_RIGHT;
			}
			else if (vx >= MARIO_RUNNING_SPEED)
			{
				state = MARIO_SMALL_STATE_RUNNING_RIGHT;
				if (isHolding)
					state = MARIO_SMALL_STATE_HOLDING_TURTLE_RUNNING_RIGHT;
			}
		}
		/*if (isHolding)
			state = MARIO_SMALL_STATE_HOLDING_TURTLE_WALK_RIGHT;*/
		if (lastnx == -1 && vx <= -MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_SMALL_STATE_STOP_LEFT);
			if (isHolding)
				state = MARIO_SMALL_STATE_HOLDING_TURTLE_WALK_RIGHT;
		}
		if (isJumping)
		{
			state = MARIO_SMALL_STATE_JUMP_RIGHT;
			if (isHolding)
				state = MARIO_SMALL_STATE_HOLDING_TURTLE_JUMP_RIGHT;
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
	case MARIO_SMALL_STATE_WALKING_LEFT:
		if (isRunning)
		{
			vx -= MARIO_RUNNING_ACC * dt;
			if (vx < -MARIO_RUNNING_SPEED)
				vx = -MARIO_RUNNING_SPEED;
			if (vx > -MARIO_RUNNING_SPEED)
			{
				state = MARIO_SMALL_STATE_WALKING_LEFT;
				if (isHolding)
					state = MARIO_SMALL_STATE_HOLDING_TURTLE_WALK_LEFT;
			}
			else if (vx <= -MARIO_RUNNING_SPEED)
			{
				state = MARIO_SMALL_STATE_RUNNING_LEFT;
				if (isHolding)
					state = MARIO_SMALL_STATE_HOLDING_TURTLE_RUNNING_LEFT;
			}
		}
		/*if (isHolding)
			state = MARIO_SMALL_STATE_HOLDING_TURTLE_WALK_LEFT;*/
		if (lastnx == 1 && vx >= MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_SMALL_STATE_STOP_RIGHT);
			if (isHolding)
				state = MARIO_SMALL_STATE_HOLDING_TURTLE_WALK_LEFT;
		}
		if (isJumping)
		{
			state = MARIO_SMALL_STATE_JUMP_LEFT;
			if (isHolding)
				state = MARIO_SMALL_STATE_HOLDING_TURTLE_JUMP_LEFT;
		}		
		if (!isRunning) {
			vx -= MARIO_WALKING_ACC * dt;
			if (vx < -MARIO_WALKING_SPEED)
				vx = -MARIO_WALKING_SPEED;
		}
		last_vx = vx;
		nx = -1;
		break;
	case MARIO_RACCOON_STATE_WALK_RIGHT:
		if (isFlying)
		{
			vx += MARIO_WALKING_ACC * dt;
			if (vx > 0.75)
				vx = 0.75;
			state = MARIO_RACCOON_STATE_KEEP_FLYING_RIGHT;
			if (isHolding)
				state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_RIGHT;
			nx = 1;
			return;
		}
		if (isRunning && !isFalling)
		{
			
			vx += MARIO_RUNNING_ACC * dt;
			if (vx > MARIO_RUNNING_SPEED)
			{
				vx = MARIO_RUNNING_SPEED;
				isMaxSpeed = true;
			}
			if (vx < MARIO_RUNNING_SPEED)
			{
				state = MARIO_RACCOON_STATE_WALK_RIGHT;
				if (isHolding)
					state = MARIO_RACCOON_STATE_HOLDING_TURTLE_WALK_RIGHT;
			}
			else if (vx >= MARIO_RUNNING_SPEED)
			{
				state = MARIO_RACCOON_STATE_RUNNING_RIGHT;
				if (isHolding)
					state = MARIO_RACCOON_STATE_HOLDING_TURTLE_RUNNING_RIGHT;
			}
		}
		/*if (isHolding)
			state = MARIO_RACCOON_STATE_HOLDING_TURTLE_WALK_RIGHT;*/
		if (lastnx == -1 && vx <= -MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_RACCOON_STATE_STOP_RIGHT);
			if (isHolding)
				state = MARIO_RACCOON_STATE_HOLDING_TURTLE_WALK_RIGHT;
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_RACCOON_STATE_JUMP_RIGHT;
			else
				state = MARIO_RACCOON_STATE_FALLING_RIGHT;
			if (isHolding)
				state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_RIGHT;
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
	case MARIO_RACCOON_STATE_WALK_LEFT:
		if (isFlying)
		{
			vx -= MARIO_WALKING_ACC * dt;
			if (vx < -0.75)
				vx = -0.75;
			state = MARIO_RACCOON_STATE_KEEP_FLYING_LEFT;
			if (isHolding)  //de khi bay len thi ve animation be rua
				state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_LEFT;     
			nx = -1;
			return;
		}
		if (isRunning && !isFalling)
		{
			vx -= MARIO_RUNNING_ACC * dt;
			if (vx < -MARIO_RUNNING_SPEED)
			{
				vx = -MARIO_RUNNING_SPEED;
				isMaxSpeed = true;
			}
			if (vx > -MARIO_RUNNING_SPEED)
			{
				state = MARIO_RACCOON_STATE_WALK_LEFT;
				if (isHolding)	//neu dang be rua thi ve animation be rua va di
					state = MARIO_RACCOON_STATE_HOLDING_TURTLE_WALK_LEFT;
			}
			else if (vx <= -MARIO_RUNNING_SPEED)
			{
				state = MARIO_RACCOON_STATE_RUNNING_LEFT;
				if (isHolding)	//neu dang be rua thi ve animation be rua va chay
					state = MARIO_RACCOON_STATE_HOLDING_TURTLE_RUNNING_LEFT;
			}
		}
		/*if (isHolding)
			state = MARIO_RACCOON_STATE_HOLDING_TURTLE_WALK_LEFT;*/
		if (lastnx == 1 && vx >= MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_RACCOON_STATE_STOP_LEFT);
			if (isHolding)	//khi be rua thi ko ve animation quay xe
				state = MARIO_RACCOON_STATE_HOLDING_TURTLE_WALK_LEFT;
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_RACCOON_STATE_JUMP_LEFT;
			else
				state = MARIO_RACCOON_STATE_FALLING_LEFT;
			if (isHolding)
				state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_LEFT;
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
	case MARIO_FIRE_STATE_WALK_RIGHT:
		if (isRunning)
		{
			vx += MARIO_RUNNING_ACC * dt;
			if (vx > MARIO_RUNNING_SPEED)
				vx = MARIO_RUNNING_SPEED;
			if (vx < MARIO_RUNNING_SPEED)
			{
				state = MARIO_FIRE_STATE_WALK_RIGHT;
				if (isHolding)
					state = MARIO_FIRE_STATE_HOLDING_TURTLE_WALK_RIGHT;
			}
			else if (vx >= MARIO_RUNNING_SPEED)
			{
				state = MARIO_FIRE_STATE_RUNNING_RIGHT;
				if (isHolding)
					state = MARIO_FIRE_STATE_HOLDING_TURTLE_RUNNING_RIGHT;
			}
		}
		/*if (isHolding)
			state = MARIO_FIRE_STATE_HOLDING_TURTLE_WALK_RIGHT;*/
		if (lastnx == -1 && vx <= -MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_FIRE_STATE_STOP_RIGHT);
			if (isHolding)
				state = MARIO_FIRE_STATE_HOLDING_TURTLE_WALK_RIGHT;
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_FIRE_STATE_JUMP_RIGHT;
			else
				state = MARIO_FIRE_STATE_FALLING_RIGHT;
			if (isHolding)
				state = MARIO_FIRE_STATE_HOLDING_TURTLE_JUMP_RIGHT;
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
	case MARIO_FIRE_STATE_WALK_LEFT:
		if (isRunning)
		{
			vx -= MARIO_RUNNING_ACC * dt;
			if (vx < -MARIO_RUNNING_SPEED)
				vx = -MARIO_RUNNING_SPEED;
			if (vx > -MARIO_RUNNING_SPEED)
			{
				state = MARIO_FIRE_STATE_WALK_LEFT;
				if (isHolding)
					state = MARIO_FIRE_STATE_HOLDING_TURTLE_WALK_LEFT;
			}
			else if (vx <= -MARIO_RUNNING_SPEED)
			{
				state = MARIO_FIRE_STATE_RUNNING_LEFT;
				if (isHolding)
					state = MARIO_FIRE_STATE_HOLDING_TURTLE_RUNNING_LEFT;
			}
		}
		/*if (isHolding)
			state = MARIO_FIRE_STATE_HOLDING_TURTLE_WALK_LEFT;*/
		if (lastnx == 1 && vx >= MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_FIRE_STATE_STOP_LEFT);
			if (isHolding)
				state = MARIO_FIRE_STATE_HOLDING_TURTLE_WALK_LEFT;
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_FIRE_STATE_JUMP_LEFT;
			else
				state = MARIO_FIRE_STATE_FALLING_LEFT;
			if (isHolding)
				state = MARIO_FIRE_STATE_HOLDING_TURTLE_JUMP_LEFT;
		}
		if (!isRunning) {
			vx -= MARIO_WALKING_ACC * dt;
			if (vx < -MARIO_WALKING_SPEED)
				vx = -MARIO_WALKING_SPEED;
		}
		last_vx = vx;
		nx = -1;
		break;
	case MARIO_RACCOON_STATE_JUMP_RIGHT:
		if (isSitting)
			state = MARIO_RACCOON_STATE_SITTING_RIGHT;
		if (isHolding)
			state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_RIGHT;
		break;
	case MARIO_RACCOON_STATE_JUMP_LEFT:
		if (isSitting)
			state = MARIO_RACCOON_STATE_SITTING_LEFT;
		if (isHolding)
			state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_LEFT;
		break;
	case MARIO_FIRE_STATE_JUMP_RIGHT:
		if (isSitting)
			state = MARIO_FIRE_STATE_SITTING_RIGHT;
		if (isHolding)
			state = MARIO_FIRE_STATE_HOLDING_TURTLE_JUMP_RIGHT;
		break;
	case MARIO_FIRE_STATE_JUMP_LEFT:
		if (isSitting)
			state = MARIO_FIRE_STATE_SITTING_LEFT;
		if (isHolding)
			state = MARIO_FIRE_STATE_HOLDING_TURTLE_JUMP_LEFT;
		break;
	case MARIO_BIG_STATE_JUMP_RIGHT:
		if (isSitting)
			state = MARIO_BIG_STATE_SITTING_RIGHT;
		if (isHolding)
			state = MARIO_BIG_STATE_HOLDING_TURTLE_JUMP_RIGHT;
		break;
	case MARIO_BIG_STATE_JUMP_LEFT:
		if (isSitting)
			state = MARIO_BIG_STATE_SITTING_LEFT;
		if (isHolding)
			state = MARIO_BIG_STATE_HOLDING_TURTLE_JUMP_LEFT;
		break;
	case MARIO_SMALL_STATE_JUMP_LEFT:
		if (isJumping)
		{
			vy = -MARIO_JUMP_SPEED_Y;
		}
		if (isHolding)
			state = MARIO_SMALL_STATE_HOLDING_TURTLE_JUMP_LEFT;
		break;
	case MARIO_SMALL_STATE_JUMP_RIGHT:
		if (isJumping)
		{
			vy = -MARIO_JUMP_SPEED_Y;
		}
		if (isHolding)
			state = MARIO_SMALL_STATE_HOLDING_TURTLE_JUMP_RIGHT;
		break;
	case MARIO_STATE_IDLE:
		isRunning = false;
		if(!isJumping)
			isSitting = false;
		if (level == MARIO_LEVEL_BIG)
		{
			if (vx > 0) {
				vx -= MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_BIG_STATE_WALKING_RIGHT;
				if (isHolding)
				{
					state = MARIO_BIG_STATE_HOLDING_TURTLE_IDLE_RIGHT;
					if (nx < 0)
						state = MARIO_BIG_STATE_HOLDING_TURTLE_IDLE_LEFT;
				}
			}
			if (vx < 0) {
				vx += MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx > 0)
					vx = 0;
				state = MARIO_BIG_STATE_WALKING_LEFT;
				if (isHolding)
				{
					state = MARIO_BIG_STATE_HOLDING_TURTLE_IDLE_LEFT;
					if(nx>0)
						state = MARIO_BIG_STATE_HOLDING_TURTLE_IDLE_RIGHT;
				}
			}
			if (nx > 0 && vx == 0)
			{
				state = MARIO_BIG_STATE_IDLE_RIGHT;
				if (isHolding)
					state = MARIO_BIG_STATE_HOLDING_TURTLE_IDLE_RIGHT;
			}
			else if (nx < 0 && vx == 0)
			{
				state = MARIO_BIG_STATE_IDLE_LEFT;
				if (isHolding)
					state = MARIO_BIG_STATE_HOLDING_TURTLE_IDLE_LEFT;
			}
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (vx > 0) {
				vx -= MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_SMALL_STATE_WALKING_RIGHT;
				if (isHolding)
					state = MARIO_SMALL_STATE_HOLDING_TURTLE_IDLE_RIGHT;
			}
			if (vx < 0) {
				vx += MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx > 0)
					vx = 0;
				state = MARIO_SMALL_STATE_WALKING_LEFT;
				if (isHolding)
					state = MARIO_SMALL_STATE_HOLDING_TURTLE_IDLE_LEFT;
			}
			if (nx > 0 && vx == 0)
			{
				state = MARIO_SMALL_STATE_IDLE_RIGHT;
				if (isHolding)
					state = MARIO_SMALL_STATE_HOLDING_TURTLE_IDLE_RIGHT;
			}
			else if (nx < 0 && vx == 0)
			{
				state = MARIO_SMALL_STATE_IDLE_LEFT;
				if (isHolding)
					state = MARIO_SMALL_STATE_HOLDING_TURTLE_IDLE_LEFT;
			}
		}
		else if (level == MARIO_RACCOON)
		{
			if (vx > 0) {
				vx -= MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_RACCOON_STATE_WALK_RIGHT;
				if (isHolding)
				{
					state = MARIO_RACCOON_STATE_HOLDING_TURTLE_IDLE_RIGHT;
					if(nx<0)
						state = MARIO_RACCOON_STATE_HOLDING_TURTLE_IDLE_LEFT;
				}
			}
			if (vx < 0) {
				vx += MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx > 0)
					vx = 0;
				state = MARIO_RACCOON_STATE_WALK_LEFT;
				if (isHolding)
				{
					state = MARIO_RACCOON_STATE_HOLDING_TURTLE_IDLE_LEFT;
					if (nx > 0)
						state = MARIO_RACCOON_STATE_HOLDING_TURTLE_IDLE_RIGHT;
				}
			}
			if (nx > 0 && vx == 0)
			{
				state = MARIO_RACCOON_STATE_IDLE_RIGHT;
				if (isHolding)
					state = MARIO_RACCOON_STATE_HOLDING_TURTLE_IDLE_RIGHT;
			}
			else if (nx < 0 && vx == 0)
			{
				state = MARIO_RACCOON_STATE_IDLE_LEFT;
				if (isHolding)
					state = MARIO_RACCOON_STATE_HOLDING_TURTLE_IDLE_LEFT;
			}
		}
		else if (level == MARIO_FIRE)
		{
			if (vx > 0) {
				vx -= MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_FIRE_STATE_WALK_RIGHT;
				if (isHolding)
					state = MARIO_FIRE_STATE_HOLDING_TURTLE_IDLE_RIGHT;
			}
			if (vx < 0) {
				vx += MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx > 0)
					vx = 0;
				state = MARIO_FIRE_STATE_WALK_LEFT;
				if (isHolding)
					state = MARIO_FIRE_STATE_HOLDING_TURTLE_IDLE_LEFT;
			}
			if (nx > 0 && vx == 0)
			{
				state = MARIO_FIRE_STATE_IDLE_RIGHT;
				if (isHolding)
					state = MARIO_FIRE_STATE_HOLDING_TURTLE_IDLE_RIGHT;
			}
			else if (nx < 0 && vx == 0)
			{
				state = MARIO_FIRE_STATE_IDLE_LEFT;
				if (isHolding)
					state = MARIO_FIRE_STATE_HOLDING_TURTLE_IDLE_LEFT;
			}
		}
		break;
	case MARIO_FIRE_STATE_FIGHT_RIGHT:
	case MARIO_FIRE_STATE_FIGHT_LEFT:
	case MARIO_RACCOON_STATE_FIGHT_IDLE_RIGHT:
	case MARIO_RACCOON_STATE_FIGHT_IDLE_LEFT:
	case MARIO_FIRE_STATE_FIGHT_WHILE_JUMPING_RIGHT:
	case MARIO_FIRE_STATE_FIGHT_WHILE_JUMPING_LEFT:
		ResetAni();
		isWaitingForAni = true;
		break;
	case MARIO_ANI_DIE:
		//state = MARIO_ANI_DIE;
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_BIG_STATE_SITTING_RIGHT:
	case MARIO_BIG_STATE_SITTING_LEFT:
	case MARIO_FIRE_STATE_SITTING_RIGHT:
	case MARIO_FIRE_STATE_SITTING_LEFT:
	case MARIO_RACCOON_STATE_SITTING_RIGHT:
	case MARIO_RACCOON_STATE_SITTING_LEFT:
		isSitting = true;
		//DebugOut(L"gia tri vx truoc khi tru %f \n", vx);
		SubRunningAcc();	
		//DebugOut(L"gia tri vx sau khi tru %f\n", vx);
		break;
	case MARIO_BIG_STATE_STOP_LEFT:
	case MARIO_BIG_STATE_STOP_RIGHT:
	case MARIO_SMALL_STATE_STOP_RIGHT:
	case MARIO_SMALL_STATE_STOP_LEFT:
	case MARIO_FIRE_STATE_STOP_RIGHT:
	case MARIO_FIRE_STATE_STOP_LEFT:
	case MARIO_RACCOON_STATE_STOP_LEFT:
	case MARIO_RACCOON_STATE_STOP_RIGHT:		
		if(vx>-0.15&&vx<0.15)
			SubRunningAcc();
		else 
			SubRunningMaxAcc();
		ResetAni();
		isWaitingForAni = true;
		break;
	case MARIO_BIG_STATE_RUNNING_RIGHT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_BIG_STATE_IDLE_RIGHT;
		break;
	case MARIO_BIG_STATE_RUNNING_LEFT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_BIG_STATE_IDLE_LEFT;
		break;
	case MARIO_SMALL_STATE_RUNNING_RIGHT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_SMALL_STATE_IDLE_RIGHT;
		break;
	case MARIO_SMALL_STATE_RUNNING_LEFT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_SMALL_STATE_IDLE_LEFT;
		break;
	case MARIO_RACCOON_STATE_RUNNING_RIGHT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_RACCOON_STATE_IDLE_RIGHT;
		break;
	case MARIO_RACCOON_STATE_RUNNING_LEFT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_RACCOON_STATE_IDLE_LEFT;
		break;
	case MARIO_FIRE_STATE_RUNNING_RIGHT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_FIRE_STATE_IDLE_RIGHT;
		break;
	case MARIO_FIRE_STATE_RUNNING_LEFT:
		isRunning = true;
		if (vx == 0)
			state = MARIO_FIRE_STATE_IDLE_LEFT;
		
		break;
	case MARIO_RACCOON_STATE_FALLING_ROCK_TAIL_RIGHT:
		if (isHolding)
			state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_RIGHT;
		if (isSitting)
			state = MARIO_RACCOON_STATE_SITTING_RIGHT;
		ResetAni();
		isWaitingForAni = true;
		vy = (float)vy / 2;//-MARIO_GRAVITY * dt / 2;
		
		break;
	case MARIO_RACCOON_STATE_FALLING_ROCK_TAIL_LEFT:	
		if (isHolding)
			state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_LEFT;
		if (isSitting)
			state = MARIO_RACCOON_STATE_SITTING_LEFT;
		ResetAni();
		isWaitingForAni = true;
		vy = (float)vy / 2;//-MARIO_GRAVITY*dt/2;		
		break;
	case MARIO_RACCOON_STATE_KEEP_FLYING_RIGHT:
		if (isHolding)
		{
			state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_RIGHT;
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
	case MARIO_RACCOON_STATE_KEEP_FLYING_LEFT:
		if (isHolding)
		{
			state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_LEFT;
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
	case MARIO_RACCOON_STATE_FLYING_RIGHT:
		if (isHolding)
		{
			state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_RIGHT;
			//DebugOut(L"im here flying right \n");
		}
		vy = -MARIO_JUMP_SPEED_Y;
		timestartfly = GetTickCount64();
		break;
	case MARIO_RACCOON_STATE_FLYING_LEFT:
		if (isHolding)
			state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_LEFT;
		vy = -MARIO_JUMP_SPEED_Y;
		timestartfly = GetTickCount64();
		break;
	case MARIO_BIG_STATE_KICK_RIGHT:
	case MARIO_BIG_STATE_KICK_LEFT:
	case MARIO_SMALL_STATE_KICK_RIGHT:
	case MARIO_SMALL_STATE_KICK_LEFT:
	case MARIO_RACCOON_STATE_KICK_RIGHT:
	case MARIO_RACCOON_STATE_KICK_LEFT:
	case MARIO_FIRE_STATE_KICK_RIGHT:
	case MARIO_FIRE_STATE_KICK_LEFT:
		ResetAni();
		isWaitingForAni = true;
		break;
	case MARIO_RACCOON_STATE_MOVE_IN_PIPE:
		ResetAni();
		isWaitingForAni = true;
		//vy = 0.01;
		break;
	case MARIO_BIG_STATE_HOLDING_TURTLE_WALK_RIGHT:
	case MARIO_BIG_STATE_HOLDING_TURTLE_WALK_LEFT:
		ResetAni();
		isWaitingForAni = true;
		break;
	case MARIO_STATE_IN_WORD_MAP:
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

CMario* CMario::GetInstance()
{
	if (__instance == NULL)
		__instance = new CMario();
	return __instance;
}

void CMario::SubRunningMaxAcc()
{
	if (vx > 0) {
		vx -= MARIO_SUB_RUNNING_MAX_ACC * dt;
		last_vx = vx;
		if (vx < 0)
			vx = 0;
	}
	else if (vx < 0) {
		vx += MARIO_SUB_RUNNING_MAX_ACC * dt;
		last_vx = vx;
		if (vx > 0)
			vx = 0;
	}
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{


	/*if (state == MARIO_ANI_DIE)
	{
		left = x + 12;
		top = y-10;
		right = left + MARIO_SMALL_BBOX_WIDTH - 3;
		bottom = top + 16;

	}
	else*/ if (level == MARIO_LEVEL_BIG)
	{
		left = x;
		top = y+3;		
		bottom = top + MARIO_BIG_BBOX_HEIGHT;
		if (nx > 0)
		{
			left = x + MARIO_BIG_BBOX_LEFT+5;
			right = x + MARIO_BIG_BBOX_WIDTH_RIGHT+5;

		}
		else
		{
			left = x + MARIO_BIG_BBOX_LEFT + 5;
			right = x + MARIO_BIG_BBOX_WIDTH+8;
			if (isHolding)
			{
				left = x + MARIO_BIG_BBOX_LEFT + 2;
				right = x + MARIO_BIG_BBOX_WIDTH + 5;
			}
		}
		if (isSitting)
		{
			top = y + MARIO_SIT_BBOX+3;
			bottom = top + MARIO_BIG_SIT_BBOX_HEIGHT;
			topOfMario = top;
		}
		topOfMario = top;
		leftOfMario = left;
		bottomOfMario = bottom;
		rightOfMario = right;
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		left = x+12;
		top = y+14;
		right = left + MARIO_SMALL_BBOX_WIDTH-3;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
		topOfMario = top;
		leftOfMario = left;
		bottomOfMario = bottom;
		rightOfMario = right;
	}
	else if (level == MARIO_RACCOON)
	{
		left = x;
		top = y+2;
		bottom = top + MARIO_RACCOON_BBOX_HEIGHT;
		right = left + MARIO_RACCOON_BBOX_WIDTH;	
		topOfMario = top;
		if (isSitting)
		{
			top = y + MARIO_RACCOON_BBOX_SIT+2;
			bottom = top + MARIO_RACCOON_SIT_BBOX_HEIGHT;
			topOfMario = top;
		}
		if (nx > 0)
		{
			left = x + MARIO_SIT_BBOX+3;
			if (isHolding)
				left = x + MARIO_SIT_BBOX;
			right = left + MARIO_RACCOON_BBOX_WIDTH_RIGHT;
		}
		else
		{
			left = x + MARIO_RACCOON_BBOX_LEFT+1;
			if (isHolding)
				left = x;
			right = left + MARIO_RACCOON_BBOX_WIDTH_RIGHT;
		}
		topOfMario = top;
		leftOfMario = left;
		bottomOfMario = bottom;
		rightOfMario = right;
	}
	else if (level == MARIO_FIRE)
	{
		left = x;
		top = y+3;
		right = left + MARIO_FIRE_BBOX_WIDTH;
		bottom = top + MARIO_FIRE_BBOX_HEIGHT;
		topOfMario = top;
		if (nx > 0)
		{
			left = x + MARIO_SIT_BBOX + 3;
			if (isHolding)
				left = x + MARIO_SIT_BBOX;
			right = left + MARIO_RACCOON_BBOX_WIDTH_RIGHT;
		}
		else
		{
			left = x + MARIO_RACCOON_BBOX_LEFT+2;
			if (isHolding)
				left = x;
			right = left + MARIO_RACCOON_BBOX_WIDTH_RIGHT;
		}
		if (isSitting)
		{
			top = y + MARIO_SIT_BBOX+3;
			bottom = top + MARIO_FIRE_SIT_BBOX_HEIGHT;
			topOfMario = top;
		}
		topOfMario = top;
		leftOfMario = left;
		bottomOfMario = bottom;
		rightOfMario = right;
	}

	//right = left+ 300;

}
int CMario::GetLevel()
{
	return level;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_RACCOON);
	SetPosition(700,300 );
	SetSpeed(0, 0);
}
