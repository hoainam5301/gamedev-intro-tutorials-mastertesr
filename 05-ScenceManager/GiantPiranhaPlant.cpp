#include "GiantPiranhaPlant.h"
#include "Utils.h"


CGiantPiranhaPlant::CGiantPiranhaPlant(CMario* mario) : CGameObject()
{
	Mario=mario;
}


void CGiantPiranhaPlant::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*if (Mario->standOnPipe)
		return;*/
	CGameObject::Update(dt, coObjects);
	y += dy;
	if (start_y == 0) start_y = y;
	if (start_x == 0) start_x = x;
	
	////y += dy;
	if (y > start_y - GIANT_BOX_HEIGHT && y < start_y)
		vy += MARIO_GRAVITY / 3 * dt;
	else vy = 0;

	if (id_giantpiranha == GIANT_PIRANHA_RED)
	{
		if ((Mario->y < y && y == start_y && Mario->leftOfMario>(start_x-8) && Mario->leftOfMario < (start_x+24)) ||
			(Mario->leftOfMario > (start_x-25) && Mario->leftOfMario < (start_x+30) && y == start_y))
		{
			timewaittoshoot = GetTickCount64();
			goto Continue;
		}
		if (moveup && GetTickCount64() - timewaittoshoot > TIME_MOVE_UP)
		{
			fight = false;
			if (x - Mario->x > 0)
				SetState(GIANT_STATE_MOVE_UP_LEFT);
			else
				SetState(GIANT_STATE_MOVE_UP_RIGHT);						
		}
		else if (GetTickCount64() - timewaittoshoot > TIME_WAIT_TO_SHOT && !fight)
		{
			if (x - Mario->x > 0)
			{
				if (start_y-GIANT_BOX_HEIGHT - Mario->y > 0)
					SetState(GIANT_STATE_SHOOT_45_MORE_LEFT);
				else if (start_y - GIANT_BOX_HEIGHT - Mario->y < 0)
					SetState(GIANT_STATE_SHOOT_45_LEFT);
			}
			else
			{				
				if (start_y - GIANT_BOX_HEIGHT - Mario->y > 0)
					SetState(GIANT_STATE_SHOOT_45_MORE_RIGHT);
				else if (start_y - GIANT_BOX_HEIGHT - Mario->y < 0)
					SetState(GIANT_STATE_SHOOT_45_RIGHT);
			}
			CFireball* fireball = new CFireball({ x,start_y - GIANT_BOX_HEIGHT }, 1,Mario);
			if (x - Mario->x > 0)
				fireball->nx = -1;
			else
				fireball->nx = 1;
			if (start_y -GIANT_BOX_HEIGHT- Mario->y < 0)
				fireball->isbottom = true;
			else
				fireball->istop = true;
			listFireBall.push_back(fireball);
			fight = true;
		}
		else if (GetTickCount64() - timewaittoshoot > TIME_MOVE_DOWN && !moveup)
		{
			if (x - Mario->x > 0)
				SetState(GIANT_STATE_MOVE_DOWN_LEFT);
			else
				SetState(GIANT_STATE_MOVE_DOWN_RIGHT);
			timewaittoshoot = GetTickCount64();
		}
	}
	else if (id_giantpiranha == GIANT_PIRANHA_GREEN)
	{
		if ((Mario->y < y && y == start_y && Mario->leftOfMario>(start_x - 8) && Mario->leftOfMario < (start_x + 24)) ||
			(Mario->leftOfMario > (start_x - 25) && Mario->leftOfMario < (start_x + 30) && y == start_y))
		{
			timewaittoshoot = GetTickCount64();
			goto Continue;
		}
		if (moveup && GetTickCount64() - timewaittoshoot > TIME_MOVE_UP_GREEN)
		{
			fight = false;
			if (x - Mario->x > 0)
				SetState(GIANT_GREEN_STATE_MOVE_UP_LEFT);
			else
				SetState(GIANT_GREEN_STATE_MOVE_UP_RIGHT);
		}
		else if (GetTickCount64() - timewaittoshoot > TIME_WAIT_TO_SHOT_GREEN && !fight)
		{
			if (x - Mario->x > 0)
			{
				if (start_y - GIANT_GREEN_BOX_HEIGHT - Mario->y > 0)
					SetState(GIANT_GREEN_STATE_SHOOT_45_MORE_LEFT);
				else if (start_y - GIANT_GREEN_BOX_HEIGHT - Mario->y < 0)
					SetState(GIANT_GREEN_STATE_SHOOT_45_LEFT);
			}
			else
			{
				if (start_y - GIANT_GREEN_BOX_HEIGHT - Mario->y > 0)
					SetState(GIANT_GREEN_STATE_SHOOT_45_MORE_RIGHT);
				else if (start_y - GIANT_GREEN_BOX_HEIGHT - Mario->y < 0)
					SetState(GIANT_GREEN_STATE_SHOOT_45_RIGHT);
			}
			CFireball* fireball = new CFireball({ x,start_y - GIANT_GREEN_BOX_HEIGHT }, 1,Mario);
			if (x - Mario->x > 0)
				fireball->nx =-1 ;
			else
				fireball->nx =1;
			if (start_y - Mario->y < 0)
				fireball->isbottom = true;
			else
				fireball->istop = true;
			listFireBall.push_back(fireball);
			fight = true;
		}
		else if (GetTickCount64() - timewaittoshoot > TIME_MOVE_DOWN_GREEN && !moveup)
		{
			if (x - Mario->x > 0)
				SetState(GIANT_GREEN_STATE_MOVE_DOWN_LEFT);
			else
				SetState(GIANT_GREEN_STATE_MOVE_DOWN_RIGHT);
			timewaittoshoot = GetTickCount64();
		}
	}

	if (id_giantpiranha == GIANT_PIRANHA_RED)
	{
		if (y >= start_y && start_y != 0)
		{
			y = start_y;

		}
		if (y <= start_y - GIANT_BOX_HEIGHT && start_y != 0)
		{
			y = start_y - GIANT_BOX_HEIGHT;
		}
	}
	if (id_giantpiranha == GIANT_PIRANHA_GREEN)
	{
		if (y >= start_y && start_y != 0)
		{
			y = start_y;

		}
		if (y <= start_y - GIANT_GREEN_BOX_HEIGHT && start_y != 0)
		{
			y = start_y - GIANT_GREEN_BOX_HEIGHT;
		}
	}
	Continue:
	for (int i = 0; i < listFireBall.size(); i++)
	{
		listFireBall[i]->Update(dt, coObjects);
	}
}
void CGiantPiranhaPlant::Render()
{
	animation_set->at(state)->Render(x, y);
	for (int i = 0; i < listFireBall.size(); i++)
	{
		listFireBall[i]->Render();
	}
}

void CGiantPiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GIANT_STATE_SHOOT_45_LEFT:
		break;
	case GIANT_STATE_SHOOT_45_MORE_LEFT:
		break;
	case GIANT_STATE_MOVE_UP_LEFT:
		vy = -MOVE_SPEED;
		moveup = false;
		break;
	case GIANT_STATE_MOVE_DOWN_LEFT:
		vy = MOVE_SPEED;
		moveup = true;
		break;
	case GIANT_STATE_SHOOT_45_RIGHT:
		break;
	case GIANT_STATE_SHOOT_45_MORE_RIGHT:
		break;
	case GIANT_STATE_MOVE_UP_RIGHT:
		vy = -MOVE_SPEED;
		moveup = false;
		break;
	case GIANT_STATE_MOVE_DOWN_RIGHT:
		vy = MOVE_SPEED;
		moveup = true;
		break;
	case GIANT_GREEN_STATE_SHOOT_45_LEFT:
		break;
	case GIANT_GREEN_STATE_SHOOT_45_MORE_LEFT:
		break;
	case GIANT_GREEN_STATE_MOVE_UP_LEFT:
		vy = -MOVE_SPEED;
		moveup = false;
		break;
	case GIANT_GREEN_STATE_MOVE_DOWN_LEFT:
		vy = MOVE_SPEED;
		moveup = true;
		break;
	case GIANT_GREEN_STATE_SHOOT_45_RIGHT:
		break;
	case GIANT_GREEN_STATE_SHOOT_45_MORE_RIGHT:
		break;
	case GIANT_GREEN_STATE_MOVE_UP_RIGHT:
		vy = -MOVE_SPEED;
		moveup = false;
		break;
	case GIANT_GREEN_STATE_MOVE_DOWN_RIGHT:
		vy = MOVE_SPEED;
		moveup = true;
		break;
	}
}

void CGiantPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x;
	top = y;
	right = x + GIANT_BOX_WIDTH;
	bottom = y + GIANT_BOX_HEIGHT;

}

