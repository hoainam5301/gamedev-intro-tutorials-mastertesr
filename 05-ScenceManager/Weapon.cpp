#include "Weapon.h"
#include "Math.h"
#include "Mario.h"

CWeapon::CWeapon(float start_x, float start_y,CMario* mario)
{
	this->start_x = start_x;
	this->start_y = start_y;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(9));
	this->nx = mario->nx;
}

void CWeapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*CMario* a = new CMario();
	DebugOut(L"hiiiiiiiiiiiii %d \n", a->GetState());
	if (a->GetState() == MARIO_FIRE_ANI_FIGHT_RIGHT)
		vx = 0.1;
	else if (a->GetState() == MARIO_FIRE_ANI_FIGHT_LEFT)
		vx = -0.1;*/
	if (this->nx> 0)
		vx = 0.1;
	else
		vx = -0.1;

	if (this->y == start_y)
		vy = 0.15;
	//vy = 0.8 * sin(x);
	//else if(this->y)
	CGameObject::Update(dt);
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
		//chamsan = true;
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!


		y += min_ty * dy + ny * 0.5f;
		x += min_tx * dx + nx * 0.5f;
		if ( this->ny< 0)
		{
			vy = -0.15;
		}


		if (coEvents.size() <= 3)
		{
			x += dx;
		}
		else
			//doihuong *= -1;
		if (nx != 0) vx = 0;

		/*if (ny==0 && nx!=0)
		{
			doihuong *= -1;
			x += nx * 2;
		}*/



		//Collision logic with other objects

   /*	for (UINT i = 0; i < coEventsResult.size(); i++)
	   {
		   LPCOLLISIONEVENT e = coEventsResult[i];
		   if (dynamic_cast<CBrick*>(e->obj))
		   {

			   if (e->nx != 0)
				   doihuong *= -1;
		   }

	   }*/
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CWeapon::Render()
{
	//DebugOut(L"state %d\n", state);
	//if (!isdone)
		animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}
void CWeapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	if (!isdone)
	{
		left = x;
		top = y;
		right = x + 8;
		bottom = y + 8;
	}
	else
	{
		left = right;
		top = bottom;
	}

}