#include "Brick.h"
#include "Items.h"
#include "Utils.h"


void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (id_brick_items == ID_GACH_RA_ITEMS && bottom_coll == 1)
	{
		this->y += dy;
		
		if (this->y == height && move==false)
			vy = -0.05;
		else if (this->y <= height-BRICK_MOVE_UP && move==false)
		{			
			vy = GRAVITY;
			move = true;
		}	
		else if (this->y >= height && move == true)
		{			
			this->y = height;	
			vy = 0;
			bouncing = 1;
		}		
	}
}
	

	

CBrick::CBrick(float width, float height)
{
	this->width = width;
	this->height = height;
}

void CBrick::Render()
{	
	if(!created_item)
		animation_set->at(QUESTION_BRICK)->Render(x, y);
	else 
		animation_set->at(BRICK_AFTER)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}