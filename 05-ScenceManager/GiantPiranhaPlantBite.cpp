#include "GiantPiranhaPlantBite.h"
#include "Utils.h"


CGiantPiranhaPlantBite::CGiantPiranhaPlantBite() : CGameObject()
{
	
}


void CGiantPiranhaPlantBite::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);
	y += dy;
	if (start_y == 0) start_y = y;

	////y += dy;
	if (y > start_y - GIANT_BBOX_HEIGHT && y < start_y)
		vy += MARIO_GRAVITY / 3 * dt;
	else vy = 0;

	
		if (moveup && GetTickCount64() - timewaittoshoot > 1500)
		{
			//fight = false;
			SetState(GIANT_STATE_MOVE_UP);
			
		}	
		else if (GetTickCount64() - timewaittoshoot > 3000 && !moveup)
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
		vy = -0.15;
		moveup = false;
		break;
	case GIANT_STATE_MOVE_DOWN:
		vy = 0.15;
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

