#include "Brick.h"
#include "Items.h"
#include "Utils.h"
#define BRICK_AFTER 1
#define QUESTION_BRICK 0
#define GRAVITY 0.3f
#define	ID_GACH_RA_ITEMS	2
#define BRICK_MOVE_UP 80 //do cao cuc gay nay len
#define BRICK_MOVE_DOWN 72//vi tri cuc gach sau khi roi xuong lai

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (id_brick_items == ID_GACH_RA_ITEMS && bottom_coll == 1)
	{
		this->y += dy;
		
		if (this->y == BRICK_MOVE_UP && move==false)
			vy = -0.05;
		else if (this->y <= BRICK_MOVE_DOWN && move==false)
		{
			
			vy = GRAVITY;
			move = true;
		}	
		else if (this->y >= BRICK_MOVE_UP && move == true)
		{			
			this->y = BRICK_MOVE_UP;	
			vy = 0;
			bouncing = 1;
		}
		//DebugOut(L" yyyyy %f \n", this->y);
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