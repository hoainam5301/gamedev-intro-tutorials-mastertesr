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

	// Simple fall down
	vy += MARIO_GRAVITY * dt;
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
	if (isWaitingForAni && animation_set->at(state)->IsOver())
	{
		isWaitingForAni = false;
	}
	if (vy > 0.04)
	{
		isFalling = true;
	}
	/*else
	{
		isFalling = false;
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



		if (nx != 0) vx = last_vx;
		if (ny != 0)
		{
			if (ny < 0)
			{
				isJumping = false;
				is_grounded = true;
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

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
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
				CBrick* Brick = dynamic_cast<CBrick*>(e->obj);
				if (e->ny > 0 && Brick->id_brick_items == 2 )
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
		}
	}
	//DebugOut(L"isWaitingForAni = %d\n", isWaitingForAni);
	// clean up collision events
	//DebugOut(L"vy = %f\n", vy);
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(state)->Render(x, y, alpha);
	DebugOut(L"stataaaaaa %d\n", state);
	//RenderBoundingBox();
}


void CMario::vachamvoiitems(vector<LPGAMEOBJECT>* coObjects)
{
	float l, t, r, b, ln, tn, rn, btn;
	GetBoundingBox(l, t, r, b);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		e->GetBoundingBox(ln, tn, rn, btn);
		if (CGameObject::CheckAABB(l, t, r, b, ln, tn, rn, btn))
		{
			
			if (e->id_items==CUC_NAM)
			{
				e->isdone = true;
				y -= 20;
				level = MARIO_LEVEL_BIG;
			}
			else if (e->id_items == CHIEC_LA)
			{
				y -= 5;
				e->isdone=true;
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
		if (lastnx == -1 && vx <= -0.06)
		{
			SetState(MARIO_ANI_BIG_STOP_RIGHT);
		}
		if(isJumping)
			state=MARIO_ANI_BIG_JUMP_RIGHT;
		/*if (!is_grounded)
		{
			if (isJumping)
  				state = MARIO_ANI_BIG_JUMP_RIGHT;
			else 
				state = MARIO_ANI_BIG_FALLING_RIGHT;
		}*/
		vx += MARIO_WALKING_ACC * dt;
		if (isRunning)
		{

			if (vx < 0.175)
				state = MARIO_ANI_BIG_WALKING_RIGHT;
			else if (vx > 0.175)
				state = MARIO_ANI_BIG_RUNNING_RIGHT;
		}
		else
		{
			if (vx > 0.175f)
				vx = 0.175f;
		}
		last_vx = vx;
		nx = 1;
		break;
	case MARIO_ANI_BIG_WALKING_LEFT:
		//this->state = 1;
		if (lastnx == 1 && vx >= 0.06f)
		{
			SetState(MARIO_ANI_BIG_STOP_LEFT);
		}
		if (isJumping && isFalling)
		{
			state = MARIO_ANI_BIG_JUMP_LEFT;
		}
		vx -= MARIO_WALKING_ACC * dt;
		if (isRunning)
		{
			if (vx >- 0.2)
				state = MARIO_ANI_BIG_WALKING_LEFT;
			else if (vx <- 0.2)
				state = MARIO_ANI_BIG_RUNNING_LEFT;
		}
		else
		{
			if (vx < -0.2f)
				vx = -0.2f;
		}
		last_vx = vx;
		nx = -1;
		break;
	case MARIO_ANI_SMALL_WALKING_RIGHT:
		if (lastnx == -1 && vx <= -0.06f)
		{
			SetState(MARIO_ANI_SMALL_STOP_LEFT);
		}
		if (isJumping)
		{
			state = MARIO_ANI_SMALL_JUMP_RIGHT;
		}
		vx += MARIO_WALKING_ACC * dt;
		if (isRunning)
		{
			
		if (vx < 0.2)
			state = MARIO_ANI_SMALL_WALKING_RIGHT;
		else if (vx > 0.2)
			state = MARIO_ANI_SMALL_RUNNING_RIGHT;
		}
		else
		{
			if (vx > 0.2f)
				vx = 0.2f;
		}
		//vx = MARIO_WALKING_SPEED;
		last_vx = vx;
		nx = 1;
		break;
	case MARIO_ANI_SMALL_WALKING_LEFT:
		if (lastnx == 1 && vx >= 0.06f)
		{
			SetState(MARIO_ANI_SMALL_STOP_LEFT);
		}
		if (isJumping)
		{
			state = MARIO_ANI_SMALL_JUMP_LEFT;
		}
		vx -= MARIO_WALKING_ACC * dt;
		last_vx = vx;
		if (isRunning)
		{

			if (vx >- 0.2)
				state = MARIO_ANI_SMALL_WALKING_LEFT;
			else if (vx <- 0.2)
				state = MARIO_ANI_SMALL_RUNNING_LEFT;
		}
		else
		{
			if (vx < -0.2f)
				vx = -0.2f;
		}	
		/*if (vx < -0.2f)
			vx = -0.2f;*/
		nx = -1;
		break;
	case MARIO_RACCON_ANI_WALK_RIGHT:

		if (lastnx == -1 && vx <= -0.06)
		{
			SetState(MARIO_RACCON_ANI_STOP_RIGHT);
		}
		if (isJumping)
		{
			state = MARIO_RACCON_ANI_JUMP_RIGHT;
		}
		vx += MARIO_WALKING_ACC * dt;
		if (isRunning)
		{

			if (vx < 0.2)
				state = MARIO_RACCON_ANI_WALK_RIGHT;
			else if (vx > 0.2)
				state = MARIO_RACCON_ANI_RUNNING_RIGHT;
		}
		else
		{
			if (vx > 0.2f)
				vx = 0.2f;
		}
		//vx = MARIO_WALKING_SPEED;
		last_vx = vx;
		nx = 1;
		break;
	case MARIO_RACCON_ANI_WALK_LEFT:
		if (lastnx == 1 && vx >= 0.06)
		{
			SetState(MARIO_RACCON_ANI_STOP_LEFT);
		}
		if (isJumping)
		{
			state = MARIO_RACCON_ANI_JUMP_LEFT;
		}
		vx -= MARIO_WALKING_ACC * dt;
		if (isRunning)
		{

			if (vx > -0.2)
				state = MARIO_RACCON_ANI_WALK_LEFT;
			else if (vx < -0.2)
				state = MARIO_RACCON_ANI_RUNNING_LEFT;
		}
		else
		{
			if (vx < -0.2f)
				vx = -0.2f;
		}
		last_vx = vx;
		nx = -1;
		break;
	case MARIO_FIRE_ANI_WALK_RIGHT:
		if (lastnx == -1 && vx <= -0.06)
		{
			SetState(MARIO_FIRE_ANI_STOP_RIGHT);
		}
		if (isJumping)
		{
			state = MARIO_FIRE_ANI_JUMP_RIGHT;
		}
		vx += MARIO_WALKING_ACC * dt;
		if (isRunning)
		{

			if (vx < 0.2)
				state = MARIO_FIRE_ANI_WALK_RIGHT;
			else if (vx > 0.2)
				state = MARIO_FIRE_ANI_RUNNING_RIGHT;
		}
		else
		{
			if (vx > 0.2f)
				vx = 0.2f;
		}
		last_vx = vx;
		nx = 1;
		break;
	case MARIO_FIRE_ANI_WALK_LEFT:
		if (lastnx == 1 && vx >= 0.06)
		{
			SetState(MARIO_FIRE_ANI_STOP_LEFT);
		}
		if (isJumping)
		{
			state = MARIO_FIRE_ANI_JUMP_LEFT;
		}
		vx -= MARIO_WALKING_ACC * dt;
		if (isRunning)
		{

			if (vx > -0.2)
				state = MARIO_FIRE_ANI_WALK_LEFT;
			else if (vx < -0.2)
				state = MARIO_FIRE_ANI_RUNNING_LEFT;
		}
		else
		{
			if (vx < -0.2f)
				vx = -0.2f;
		}
		last_vx = vx;
		nx = -1;
		break;
	case MARIO_RACCON_ANI_JUMP_RIGHT:
		if (isSitting)
			state = MARIO_RACCON_ANI_SITTING_RIGHT;		
	case MARIO_RACCON_ANI_JUMP_LEFT:
		if (isSitting)
			state = MARIO_RACCON_ANI_SITTING_LEFT;		
	case MARIO_FIRE_ANI_JUMP_RIGHT:
		if (isSitting)
			state = MARIO_FIRE_ANI_SITTING_RIGHT;		
	case MARIO_FIRE_ANI_JUMP_LEFT:
		if (isSitting)
			state = MARIO_FIRE_ANI_SITTING_LEFT;		
	case MARIO_ANI_BIG_JUMP_RIGHT:
		if (isSitting)
			state = MARIO_ANI_BIG_SITTING_RIGHT;
	case MARIO_ANI_BIG_JUMP_LEFT:
		if (isSitting)
			state = MARIO_ANI_BIG_SITTING_LEFT;
	case MARIO_ANI_SMALL_JUMP_LEFT:		
	case MARIO_ANI_SMALL_JUMP_RIGHT:		
		if(isJumping)
		{
			vy = -0.275;
		}		
		/*else
			vy += MARIO_GRAVITY * dt;*/
		break;
	case MARIO_STATE_IDLE:
		isRunning = false;
		isSitting = false;
		if (level == MARIO_LEVEL_BIG)
		{
			if (vx > 0) {
				vx -= 0.0005 * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_ANI_BIG_WALKING_RIGHT;
			}
			if (vx < 0) {
				vx += 0.0005 * dt;
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
				vx -= 0.0005 * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_ANI_SMALL_WALKING_RIGHT;
			}
			if (vx < 0) {
				vx += 0.0005 * dt;
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
				vx -= 0.0005 * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_RACCON_ANI_WALK_RIGHT;
			}
			if (vx < 0) {
				vx += 0.0005 * dt;
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
				vx -= 0.0005 * dt;
				last_vx = vx;
				if (vx < 0)
					vx = 0;
				state = MARIO_FIRE_ANI_WALK_RIGHT;
			}
			if (vx < 0) {
				vx += 0.0005 * dt;
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
		if (vx > 0) {
			vx -= 0.009 * dt;
			last_vx = vx;
			if (vx < 0)
				vx = 0;
			//state = MARIO_RACCON_ANI_SITTING_RIGHT;
		}
		if (vx < 0) {
			vx += 0.009 * dt;
			last_vx = vx;
			if (vx < 0)
				vx = 0;
		}
		DebugOut(L"gia tri vx=== %f \n", vx);
		//vy = 0;
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
		if (vx > 0) {
			vx -= 0.009 * dt;
			last_vx = vx;
			if (vx < 0)
				vx = 0;
			//state = MARIO_ANI_STOP_LEFT;
		}
		else if (vx < 0) {
			vx += 0.009 * dt;
			last_vx = vx;
			if (vx < 0)
				vx = 0;
			//state = MARIO_ANI_STOP_RIGHT;
		}
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
  		ResetAni();
		isWaitingForAni = true;
		vy -= (MARIO_GRAVITY + 0.008)*dt;
		break;
	}
	//DebugOut(L"gia tri vx %d \n", state);
}

void CMario::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	

	if (level == MARIO_LEVEL_BIG)
	{
		left = x;
		top = y;
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if(level==MARIO_LEVEL_SMALL)
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
		right = left + MARIO_RACCON_BBOX_WIDTH;		
		bottom = top + MARIO_RACCON_BBOX_HEIGHT;
		if (isSitting == true)
		{
			top = y+9;
			bottom = top + MARIO_RACCON_SIT_BBOX_HEIGHT;
		}
		if (nx > 0)
		{
			left = x + 7;
			right = left + 14;
		}
	}
	else if (level == MARIO_FIRE)
	{
		left = x;
		top = y;
		right = left + MARIO_FIRE_BBOX_WIDTH;
		bottom = top + MARIO_FIRE_BBOX_HEIGHT;
		/*if (isSitting == true)
		{
			top = y + 9;
			bottom = top + MARIO_RACCON_SIT_BBOX_HEIGHT;
		}*/
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

