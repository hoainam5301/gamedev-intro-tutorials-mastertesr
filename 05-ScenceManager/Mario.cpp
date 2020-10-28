#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Items.h"
#include "Brick.h"
#include "GiantPiranhaPlant.h"
#include "Floor.h"
#include "ColorBox.h"

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
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
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
				//gravity_raccon = false;
				//isFalling = false;
			}
			vy = 0;
		}
		//if (isHand_onkey && (GetTickCount() - Start_on_Key) >= 100)
		//{
		//	isHigh_Jumping = true;
		//	//isHand_onkey = false;
		//	SetState(MARIO_ANI_SMALL_JUMP_RIGHT);
		//	DebugOut(L"already\n");
		//}

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
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
				else if (state == MARIO_RACCON_ANI_FIGHT_IDLE_RIGHT && e->nx > 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						//vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
			} // if Goomba //aabb
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				isFalling = false;
				CBrick* Brick = dynamic_cast<CBrick*>(e->obj);
				if (e->ny > 0 && Brick->id_brick_items == ID_GACH_RA_ITEMS)
				{
					Brick->bottom_coll = 1;
					//item_animation_set = 5;
				}
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
					DebugOut(L"Aaaaaaa");
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
				state = MARIO_ANI_BIG_WALKING_RIGHT;
			else if (vx >= MARIO_RUNNING_SPEED)
				state = MARIO_ANI_BIG_RUNNING_RIGHT;

		}
		if (lastnx == -1 && vx <= -MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_ANI_BIG_STOP_RIGHT);
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_ANI_BIG_JUMP_RIGHT;
			else
				state = MARIO_ANI_BIG_FALLING_RIGHT;
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
				state = MARIO_ANI_BIG_WALKING_LEFT;
			else if (vx <= -MARIO_RUNNING_SPEED)
				state = MARIO_ANI_BIG_RUNNING_LEFT;
		}
		if (lastnx == 1 && vx >= MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_ANI_BIG_STOP_LEFT);
		}
		if (isJumping )
		{
			if (vy < 0)
				state = MARIO_ANI_BIG_JUMP_LEFT;
			else
				state = MARIO_ANI_BIG_FALLING_LEFT;
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
				state = MARIO_ANI_SMALL_WALKING_RIGHT;
			else if (vx >= MARIO_RUNNING_SPEED)
				state = MARIO_ANI_SMALL_RUNNING_RIGHT;
		}
		if (lastnx == -1 && vx <= -MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_ANI_SMALL_STOP_LEFT);
		}
		if (isJumping)
		{
			state = MARIO_ANI_SMALL_JUMP_RIGHT;
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
				state = MARIO_ANI_SMALL_WALKING_LEFT;
			else if (vx <= -MARIO_RUNNING_SPEED)
				state = MARIO_ANI_SMALL_RUNNING_LEFT;
		}
		if (lastnx == 1 && vx >= MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_ANI_SMALL_STOP_LEFT);
		}
		if (isJumping)
		{
			state = MARIO_ANI_SMALL_JUMP_LEFT;
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
				state = MARIO_RACCON_ANI_WALK_RIGHT;
			else if (vx >= MARIO_RUNNING_SPEED)
				state = MARIO_RACCON_ANI_RUNNING_RIGHT;
		}
		if (lastnx == -1 && vx <= -MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_RACCON_ANI_STOP_RIGHT);
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_RACCON_ANI_JUMP_RIGHT;
			else
				state = MARIO_RACCON_ANI_FALLING_RIGHT;
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
				state = MARIO_RACCON_ANI_WALK_LEFT;
			else if (vx <= -MARIO_RUNNING_SPEED)
				state = MARIO_RACCON_ANI_RUNNING_LEFT;
		}
		if (lastnx == 1 && vx >= MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_RACCON_ANI_STOP_LEFT);
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_RACCON_ANI_JUMP_LEFT;
			else
				state = MARIO_RACCON_ANI_FALLING_LEFT;
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
				state = MARIO_FIRE_ANI_WALK_RIGHT;
			else if (vx >= MARIO_RUNNING_SPEED)
				state = MARIO_FIRE_ANI_RUNNING_RIGHT;
		}
		if (lastnx == -1 && vx <= -MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_FIRE_ANI_STOP_RIGHT);
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_FIRE_ANI_JUMP_RIGHT;
			else
				state = MARIO_FIRE_ANI_FALLING_RIGHT;
		}
		if (!isRunning) 
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
				state = MARIO_FIRE_ANI_WALK_LEFT;
			else if (vx <= -MARIO_RUNNING_SPEED)
				state = MARIO_FIRE_ANI_RUNNING_LEFT;
			//DebugOut(L"Gia tri vx khi running %f \n", vx);
		}
		if (lastnx == 1 && vx >= MARIO_MIN_SPEED_TO_STOP)
		{
			SetState(MARIO_FIRE_ANI_STOP_LEFT);
		}
		if (isJumping)
		{
			if (vy < 0)
				state = MARIO_FIRE_ANI_JUMP_LEFT;
			else
				state = MARIO_FIRE_ANI_FALLING_LEFT;
		}
		if (!isRunning) {
			vx -= MARIO_WALKING_ACC * dt;
			if (vx < -MARIO_WALKING_SPEED)
				vx = -MARIO_WALKING_SPEED;
			//DebugOut(L"Gia tri vx khi walking %f\n", vx);
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
			}
			if (vx < 0) {
				vx += MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx > 0)
					vx = 0;
				state = MARIO_ANI_BIG_WALKING_LEFT;
			}
			if (nx > 0 && vx == 0)
				ani = MARIO_ANI_BIG_IDLE_RIGHT;
			else if (nx < 0 && vx == 0)
				state = MARIO_ANI_BIG_IDLE_LEFT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			if (vx > 0) {
				vx -= MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_ANI_SMALL_WALKING_RIGHT;
			}
			if (vx < 0) {
				vx += MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx > 0)
					vx = 0;
				state = MARIO_ANI_SMALL_WALKING_LEFT;
			}
			if (nx > 0 && vx == 0)
				state = MARIO_ANI_SMALL_IDLE_RIGHT;
			else if (nx < 0 && vx == 0)
				state = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (level == MARIO_RACCON)
		{
			if (vx > 0) {
				vx -= MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_RACCON_ANI_WALK_RIGHT;
			}
			if (vx < 0) {
				vx += MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx > 0)
					vx = 0;
				state = MARIO_RACCON_ANI_WALK_LEFT;
			}
			if (nx > 0 && vx == 0)
				state = MARIO_RACCON_ANI_IDLE_RIGHT;
			else if (nx < 0 && vx == 0)
				state = MARIO_RACCON_ANI_IDLE_LEFT;
		}
		else if (level == MARIO_FIRE)
		{
			if (vx > 0) {
				vx -= MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_FIRE_ANI_WALK_RIGHT;
			}
			if (vx < 0) {
				vx += MARIO_SUB_WALKING_ACC * dt;
				last_vx = vx;
				if (vx > 0)
					vx = 0;
				state = MARIO_FIRE_ANI_WALK_LEFT;
			}
			if (nx > 0 && vx == 0)
				state = MARIO_FIRE_ANI_IDLE_RIGHT;
			else if (nx < 0 && vx == 0)
				state = MARIO_FIRE_ANI_IDLE_LEFT;
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
	case MARIO_STATE_DIE:
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
		ResetAni();
		isWaitingForAni = true;
		SubRunningAcc();
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
	case MARIO_RACCON_ANI_FALLING_ROCK_TAIL_LEFT:
		//state = MARIO_ANI_BIG_JUMP_RIGHT;
		ResetAni();
		isWaitingForAni = true;
		//gravity_raccon = true;
		vy = -MARIO_GRAVITY*dt/2;
		break;
	case MARIO_RACCON_ANI_KEEP_FLYING_RIGHT:
		ResetAni();
		/*if (vx > 0.3)
			vx -= MARIO_WALKING_ACC * dt;
		if (vx <= MARIO_WALKING_SPEED)
			vx = MARIO_WALKING_SPEED;*/
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
	case MARIO_RACCON_ANI_FLYING_LEFT:
		vy = -MARIO_JUMP_SPEED_Y;
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
