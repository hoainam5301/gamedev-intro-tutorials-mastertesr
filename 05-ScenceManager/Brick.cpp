#include "Brick.h"
#include "Items.h"
#include "Utils.h"
#include "Coin.h"

CBrick::CBrick(float width, float height)
{
	this->width = width;
	this->height = height;
}

void CBrick::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (bottom_coll == 1)
	{
		this->y += dy;

		if (this->y == height && move == false)
			vy = -MOVE_UP_SPEED;
		else if (this->y <= height - BRICK_MOVE_UP && move == false)
		{
			vy = GRAVITY;
			move = true;
		}
		else if (this->y >= height && move == true)
		{
			this->y = height;
			vy = 0;
			//if(id_brick_items!=ID_GACH_RA_NHIEU_TIEN)
				bouncing = 1;
		}
	}
}


void CBrick::Render()
{	
	if (!created_item)
	{
		if (id_brick_items == ID_GACH_RA_ITEMS || id_brick_items == ID_GACH_RA_TIEN)
			animation_set->at(QUESTION_BRICK)->Render(x, y);
		else if (id_brick_items == ID_GACH_SWITCH_P || id_brick_items == ID_GACH_BONUS ||id_brick_items==ID_GACH_RA_LEAF_TREA||id_brick_items==ID_GACH_RA_NHIEU_TIEN)
			animation_set->at(BRICK_MAKE_LIFE)->Render(x, y);
	}
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