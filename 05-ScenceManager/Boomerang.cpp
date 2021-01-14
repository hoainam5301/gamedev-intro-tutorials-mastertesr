#include "Boomerang.h"



CBoomerang::CBoomerang(int nx, CMario* mario,CBoomerangBrother* brother)
{
	Mario = mario;
	Brother = brother;
	this->nx = nx;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(20));
}

void CBoomerang::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);	
	if (startX == 0)
		startX = x;
	if (startY == 0)
		startY = y;
	vx = 0.1;
	vy = -0.02;
	if (x - startX > 100)
	{		
		vy = 0.1;
	}
	if (x - startX > 150)
	{
		vx = -vx;
		vy = 0.1;
	}
	if (y - startY>0  && y-startY<5)
	{		
		vy = 0.1;
		vx = -vx;
	}

	if (y - startY > 5)
	{
		//DebugOut(L"gia tri x %f \n", x);
		vy = 0;
		vx = -vx;
	}
	if (x < Brother->x)
		isdone = true;
	x += dx;
	y += dy;
	//Collision_Mario(Mario);
}

void CBoomerang::Collision_Mario(CMario* mario)
{
	float l_ball, t_ball, r_ball, b_ball, l_mario, t_mario, r_mario, b_mario;
	GetBoundingBox(l_ball, t_ball, r_ball, b_ball);
	mario->GetBoundingBox(l_mario, t_mario, r_mario, b_mario);
	if (CGameObject::CheckAABB(l_ball, t_ball, r_ball, b_ball, l_mario, t_mario, r_mario, b_mario))
	{
		/*if (!hasCollion)
		{*/
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
			//hasCollion = true;
		//}
	}
}

void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!isdone)
	{
		left = x;
		top = y;
		right = x + WIDTH_HEIGHT_BOOMERANG;
		bottom = y + WIDTH_HEIGHT_BOOMERANG;
	}
}


void CBoomerang::Render()
{
	if (!isdone)
		animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

CBoomerang::~CBoomerang()
{
}
