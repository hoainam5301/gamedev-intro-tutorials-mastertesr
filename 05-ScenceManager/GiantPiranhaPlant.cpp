#include "GiantPiranhaPlant.h"
#include "Utils.h"


CGiantPiranhaPlant::CGiantPiranhaPlant(CMario* mario) : CGameObject()
{
	Mario=mario;
}


void CGiantPiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	/*case GIANT_STATE_45:
		break;
	case GIANT_STATE_45_MORE:
		break;	*/
	//case GIANT_STATE_UP:
	//	//state = GIANT_STATE_45_MORE;
	//	vy = -0.125;
	//	break;
	//case GIANT_STATE_DOWN:
	//	//state = GIANT_STATE_45;
	//	vy = 0;
	//	break;
	case GIANT_STATE_SHOOT_45:
		//fight = false;
		break;
	case GIANT_STATE_SHOOT_45_MORE:
		//fight = false;
		break;
	case GIANT_STATE_MOVE_UP:	
		vy = -0.15f;
		moveup = false;
		break;
	case GIANT_STATE_MOVE_DOWN:		
		vy = 0.15f;
		moveup = true;
		break;
	}
}



void CGiantPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);
	y += dy;
	
	//y += (y == start_y - 32  && moveup ) ? 0 : dy;

	//if (start_y == 0) start_y = y;
	////y += dy;
	if (y > 336 && y < 368)
		vy += MARIO_GRAVITY / 3 * dt;
	else vy = 0;



	///*if (start_y == 0)
	//	start_y = y;*/
	//if (timetoshoot == 0)
	//{
	//	timetoshoot = GetTickCount64();
	//	timetomovedown = 0;
	//	// -1 mean STATE DEFAULT
	//	if (state == GIANT_STATE_DOWN || state == 0) SetState(GIANT_STATE_UP);
	//}
	//else if (GetTickCount64() - timetoshoot > 3000)
	//{
	//	if (state == GIANT_STATE_UP)
	//	{
	//		SetState(GIANT_STATE_DOWN);
	//	}
	//	if (timetomovedown == 0) timetomovedown = GetTickCount64();
	//	if (GetTickCount64() - timetomovedown > 3000)
	//	{
	//		timetoshoot = 0;
	//	}

	//}

	//
	//if (y <= start_y - 32 )
	//{
	//	y = start_y - 32;
	//}
	//
	//if (y >= start_y )
	//{
	//	y = start_y;
	//}
	if (moveup && GetTickCount64() - timewaittoshoot>1000)
	{
		fight = false;
		SetState(GIANT_STATE_MOVE_UP);
	}
	else if (GetTickCount64() - timewaittoshoot > 2000 && !fight)
	{
		if (336 - Mario->y > 0)
			SetState(GIANT_STATE_SHOOT_45_MORE);
		else if(336-Mario->y<0)
			SetState(GIANT_STATE_SHOOT_45);
		//delta_x = x - Mario->x;
		//delta_y = 336 - Mario->y;
		CFireball* fireball = new CFireball({ x,336 }, 1);
		listFireBall.push_back(fireball);
		DebugOut(L"size cua lisst fireball %d \n", listFireBall.size());
		
		
		fight = true;
		
	}
	else if (GetTickCount64() - timewaittoshoot > 3000/* && timewaittoshoot != 0*/ && !moveup)
	{
		
		SetState(GIANT_STATE_MOVE_DOWN);
		timewaittoshoot = GetTickCount64();
	}

	if (y >= 368) 
	{
		y = 368;
		
	}
	if (y <= 336)
	{		
		y = 336;
	}
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

void CGiantPiranhaPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x;
	top = y;
	right = x + GIANT_BOX_WIDTH;
	bottom = y + GIANT_BOX_HEIGHT;

}

