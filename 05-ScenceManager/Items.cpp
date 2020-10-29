#include "Items.h"
#include "Brick.h"
#include "Utils.h"
#define GRAVITY 0.3f
#define Item_move 62 // quang duong item di chuyen khi troi len
CItems::CItems(float start_x,float start_y)
{
	
	this->Start_x = start_x;
	this->Start_y = start_y;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(8));
}

void CItems::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
	if (!isdone)
	{
		left = x;
		top = y;
		right = x + 18;
		bottom = y + 18;
	}
}
void CItems::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if(spawn){
		//DebugOut(L"vy %f \n", vy);
		if (this->y >= Start_x-20)
			vy = -0.05;
		else spawn = false;

	}
	else 
	{
		vx = 0.1 * change_direction;
		//x += 0.03 * dt;
		vy = GRAVITY;

	}
	//DebugOut(L"gia tri start y %f \n", Start_y);
	//DebugOut(L"gia tri y %f \n", y);
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
		//x += dx;
		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		
		// block every object first!
		/*if (ny != 0)
		{
			vy = 0;

		}*/
		
		y += min_ty * dy + ny * 0.5f;
		x += min_tx * dx + nx * 0.5f;

		// += dx;
		/*if (coEvents.size() <= 2)
		{
			x += dx;
		}
		else
			change_direction *= -1;*/
		if (nx != 0) change_direction *= -1;
		
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
void CItems::Render()
{
	//DebugOut(L"state %d\n", state);
	if(!isdone)
		animation_set->at(state)->Render(x, y);
	//RenderBoundingBox();
}

void CItems::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case Mushroom:
		break;
	case Tree_Leaf:
		break;
	case FIRE_FLOWER:
		break;
	}
}