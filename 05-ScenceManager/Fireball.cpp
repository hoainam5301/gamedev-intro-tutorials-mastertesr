
#include "Fireball.h"
#include "Pipe.h"

CFireball::CFireball(D3DXVECTOR2 position, int nx,CMario* mario)
{

	if (nx > 0)
	{
		x = position.x + BONUS_POS_X;
		y = position.y + BONUS_POS_XY;
	}
	else
	{
		x = position.x - BONUS_POS_XY;
		y = position.y + BONUS_POS_XY;
	}
	Mario = mario;
	this->nx = nx;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(9));
}

void CFireball::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (y > 416)
		isdone = true;
	else if (Mario->standOnCloudBrick)
	{
		if (y < 300)
			isdone = true;
	}
	if (isdone)
		return;
	if (isbottom)
	{
		if (nx < 0)
		{
			vx = -MOVE_SPEED_X;
			vy = MOVE_SPEED_Y;
		}
		else
		{
			vx = MOVE_SPEED_X;
			vy = MOVE_SPEED_Y;
		}
	}
	if (istop)
	{
		if (nx < 0)
		{
			vx = -MOVE_SPEED_X;
			vy = -MOVE_SPEED_Y;
		}
		else
		{
			vx = MOVE_SPEED_X;
			vy = -MOVE_SPEED_Y;
		}
	}
	x += dx;
	y += dy;
	Collision_Mario(Mario);
}

void CFireball::Collision_Mario(CMario* mario)
{
	float l_ball, t_ball, r_ball, b_ball, l_mario, t_mario, r_mario, b_mario;
	GetBoundingBox(l_ball, t_ball, r_ball, b_ball);
	mario->GetBoundingBox(l_mario, t_mario, r_mario, b_mario);
	if (CGameObject::CheckAABB(l_ball, t_ball, r_ball, b_ball, l_mario, t_mario, r_mario, b_mario))
	{
		if (!hasCollion)
		{
			if (mario->level > MARIO_LEVEL_BIG)
			{
				mario->level = MARIO_LEVEL_BIG;
				mario->StartUntouchable();
			}
			else if (mario->level == MARIO_LEVEL_BIG)
			{
				mario->level = MARIO_LEVEL_SMALL;
				mario->StartUntouchable();
			}
			else
			{
				mario->SetState(MARIO_ANI_DIE);
				//return;
			}
			hasCollion = true;
			timeRenderFireExplore = GetTickCount64();
		}
	}
}

void CFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isdone)
	{
		left = x;
		top = y;
		right = x + WIDTH_HEIGHT_FIREBALL;
		bottom = y + WIDTH_HEIGHT_FIREBALL;
	}
}


void CFireball::Render()
{	
	if (!isdone && !hasCollion)
		animation_set->at(0)->Render(x, y);
	else if (hasCollion && !isdone)
	{
		if (GetTickCount64() - timeRenderFireExplore < 25)
			animation_set->at(1)->Render(x, y);
		else
			isdone = true;
	}
	//RenderBoundingBox();
}

CFireball::~CFireball()
{
}
