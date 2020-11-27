#include "Items.h"
#include "Brick.h"
#include "ColorBox.h"
#include "Math.h"
#include "MonneyEffect.h"

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
		right = x + 16;
		bottom = y + 16;		
	}	
}
void CItems::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	 if (id_items == Tree_Leaf)
	{
		maxRight = Start_y + 20;
		if (spawn) {

			if (this->y >= Start_x - 20)
				vy = -0.35;
			else spawn = false;
		}
		else
		{
			if (x <= Start_y )
			{
				vy = 0.35;
				vx = 0.3 * pow(35, vy);
				SetState(Tree_Leaf_move_right);
			}
			if (x >= maxRight)
			{
				vy = 0.35;
				vx = -0.3 * pow(35, vy);
				SetState(Tree_Leaf_move_left);
			}
		}
		if (makeEffect)
		{
			CMonneyEffect* monneyeffect = new CMonneyEffect();
			monneyeffect->SetPosition(x, y);
			monneyeffect->SetState(MAKE_1000);
			makeEffect = false;
			listEffect.push_back(monneyeffect);
		}
		x += vx;
		y += vy;
	}	
	 else
	 {
		 if (id_items == Mushroom)
		 {
			 if (spawn) {

				 if (this->y >= Start_x - 20)
					 vy = -0.05;
				 else spawn = false;
			 }
			 else
			 {
				 vx = 0.1 * change_direction;
				 vy = GRAVITY;
			 }
		 }
		 else if (id_items == FIRE_FLOWER)
		 {
			 if (spawn) {

				 if (this->y >= Start_x - 15)
					 vy = -0.05;
				 else spawn = false;
			 }
			 else
				 vy = 0;
		 }
		 if (makeEffect)
		 {
			 CMonneyEffect* monneyeffect = new CMonneyEffect();
			 monneyeffect->SetPosition(x, y);
			 monneyeffect->SetState(MAKE_1000);
			 makeEffect = false;
			 listEffect.push_back(monneyeffect);
		 }
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
		 	y += min_ty * dy + ny * 0.5f;
		 	x += min_tx * dx + nx * 0.5f;
		 	
		 /*	if (nx != 0)
		 	{
		 		 change_direction *= -1;
		 	}*/
		 		
		 	 //Collision logic with other objects
		 	
		 	for (UINT i = 0; i < coEventsResult.size(); i++)
		 	{
		 		LPCOLLISIONEVENT e = coEventsResult[i];
		 		if (dynamic_cast<CColorBox*>(e->obj))
		 		{
		 			if (e->nx != 0)
		 			{
		 				x += dx;
		 			}
		 		}
		 		else if (!dynamic_cast<CColorBox*>(e->obj))
		 		{
		 			if(e->nx!=0)
		 				change_direction *= -1;
		 		}

		 	}
		 }
		 
		 for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	 }
	 for (int i = 0; i < listEffect.size(); i++)
	 {
		 listEffect[i]->Update(dt, coObjects);
	 }
}
void CItems::Render()
{
	if(!isdone)
		animation_set->at(state)->Render(x, y);
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Render();
	}
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
	case Tree_Leaf_move_left:
		break;
	case Tree_Leaf_move_right:
		break;
	case SWITCH_P_ON:
		break;
	case SWITCH_P_OFF:
		break;
	}
}