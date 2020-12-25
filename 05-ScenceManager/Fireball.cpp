
#include "Fireball.h"

CFireball::CFireball(D3DXVECTOR2 position, int nx,CMario* mario)
{

	if (nx > 0)
	{
		x = position.x + 10;
		y = position.y + 6;
	}
	else
	{
		x = position.x - 6;
		y = position.y + 6;
	}
	Mario = mario;
	this->nx = nx;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(9));
}

void CFireball::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (isbottom)
	{
		if (nx < 0)
		{
			vx = -0.1;
			vy = 0.1;
		}
		else
		{
			vx = 0.1;
			vy = 0.1;
		}
	}
	if (istop)
	{
		if (nx < 0)
		{
			vx = -0.1;
			vy = -0.1;
		}
		else
		{
			vx = 0.1;
			vy = -0.1;
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
		}
	}
}

void CFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 8;
	bottom = y + 8;
}


void CFireball::Render()
{	
	animation_set->at(0)->Render(x, y);	
	//RenderBoundingBox();
}

CFireball::~CFireball()
{
}
