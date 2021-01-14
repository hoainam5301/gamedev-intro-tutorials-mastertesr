#include "GiantPiranhaPlantBite.h"
#include "Utils.h"


CGiantPiranhaPlantBite::CGiantPiranhaPlantBite(CMario* mario) : CGameObject()
{
	Mario = mario;
}


void CGiantPiranhaPlantBite::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;
	if (start_y == 0) start_y = y;
	if (start_x == 0) start_x = x;

	////y += dy;
	if (y > start_y - GIANT_BBOX_HEIGHT && y < start_y)
		vy += MARIO_GRAVITY / 3 * dt;
	else vy = 0;

	if ((Mario->y < y && y == start_y && Mario->leftOfMario>(start_x - 8) && Mario->leftOfMario < (start_x + 24)) ||
		(Mario->leftOfMario > (start_x - 25) && Mario->leftOfMario < (start_x + 30) && y == start_y))
	{
		timewaittoshoot = GetTickCount64();
		return;
	}
	
	if (moveup && GetTickCount64() - timewaittoshoot > TIME_MOVE_UP)
	{
		SetState(GIANT_STATE_MOVE_UP);
	}
	else if (GetTickCount64() - timewaittoshoot > TIME_MOVE_DOWN && !moveup)
	{
		timewaittoshoot = GetTickCount64();
		SetState(GIANT_STATE_MOVE_DOWN);
	}


	if (y >= start_y && start_y != 0)
	{
		y = start_y;

	}
	if (y <= start_y - GIANT_BBOX_HEIGHT && start_y != 0)
	{
		y = start_y - GIANT_BBOX_HEIGHT;
	}	
}
void CGiantPiranhaPlantBite::Render()
{
	animation_set->at(state)->Render(x, y);	
}

void CGiantPiranhaPlantBite::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GIANT_STATE_MOVE_UP:
		vy = -MOVE_SPEED;
		moveup = false;
		break;
	case GIANT_STATE_MOVE_DOWN:
		vy = MOVE_SPEED;
		moveup = true;
		break;	
	}
}

void CGiantPiranhaPlantBite::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x;
	top = y;
	right = x + GIANT_BBOX_WIDTH;
	bottom = y + GIANT_BBOX_HEIGHT;

}

