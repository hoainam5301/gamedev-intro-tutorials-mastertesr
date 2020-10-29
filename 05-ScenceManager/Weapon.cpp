#include "Weapon.h"
#include "Floor.h"
#include "Math.h"
#include "Koopas.h"
#include "Goomba.h"

CWeapon::CWeapon(float start_x, float start_y,int marionx)
{
	this->start_x = start_x;
	this->start_y = start_y;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(9));
	this->nx = marionx;
}

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*CMario* a = new CMario();
	DebugOut(L"hiiiiiiiiiiiii %d \n", a->GetState());
	if (a->GetState() == MARIO_FIRE_ANI_FIGHT_RIGHT)
		vx = 0.1;
	else if (a->GetState() == MARIO_FIRE_ANI_FIGHT_LEFT)
		vx = -0.1;*/
	if (state == FIRE_BALL_MOVE)
	{
		if (nx > 0)
			vx = MOVING_SPEED;
		else
			vx = -MOVING_SPEED;


		vy += (MARIO_GRAVITY * dt);
	}
	CGameObject::Update(dt);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	if (isWaitingForAni && animation_set->at(state)->IsOver())
	{
		isWaitingForAni = false;
		isExplode = true;
	}

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


		y += min_ty * dy + ny * 0.5f;
		x += min_tx * dx + nx * 0.5f;
		if (ny< 0)
		{
			vy = -MOVING_SPEED;
		}


		if (coEvents.size() <= 3)
		{
			x += dx;
		}
		else
			//doihuong *= -1;
		if (nx != 0) this->isdone=true;

		/*if (ny==0 && nx!=0)
		{
			doihuong *= -1;
			x += nx * 2;
		}*/



		//Collision logic with other objects

   	for (UINT i = 0; i < coEventsResult.size(); i++)
	   {

		   LPCOLLISIONEVENT e = coEventsResult[i];
		   if (dynamic_cast<CFloor*>(e->obj))
		   {
			   if (e->nx != 0)
				   SetState(FIRE_BALL_EXPLODE);
		   }
		   else if (dynamic_cast<CGoomba*>(e->obj))
		   {
			   CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
			   if (goomba->GetState() != GOOMBA_STATE_DIE)
			   {
				   goomba->SetState(GOOMBA_STATE_DIE);
				   SetState(FIRE_BALL_EXPLODE);
			   }
		   }
		   else if (dynamic_cast<CKoopas*>(e->obj))
		   {
			   CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
			   if (koopas->GetState() != GOOMBA_STATE_DIE)
			   {
				   koopas->SetState(GOOMBA_STATE_DIE);
				   SetState(FIRE_BALL_EXPLODE);
			   }
		   }
	   }
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CWeapon::Render()
{
	if(state==FIRE_BALL_MOVE)
		animation_set->at(state)->Render(x, y);	
	if(state==FIRE_BALL_EXPLODE && isWaitingForAni)
		animation_set->at(state)->Render(x, y);
}
void CWeapon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRE_BALL_MOVE:
		isExplode = false;
		break;
	case FIRE_BALL_EXPLODE:
		ResetAni();
		isWaitingForAni = true;		
		vx = 0;
		vy = 0;
		break;
	}
}
void CWeapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state==FIRE_BALL_MOVE)
	{
		left = x;
		top = y;
		right = x + 8;
		bottom = y + 8;
	}
	else if(state==FIRE_BALL_EXPLODE)
	{
		left = right;
		top = bottom;
	}

}