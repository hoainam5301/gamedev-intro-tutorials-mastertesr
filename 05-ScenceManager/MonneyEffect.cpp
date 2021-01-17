#include "MonneyEffect.h"


CMonneyEffect::CMonneyEffect()
{ 
	
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(15));
	
}
void CMonneyEffect::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isdone && state!=HIT_EFFECT)
	{
		CGameObject::Update(dt);
		if (start_y == 0)
			start_y = y;
		if (coinFormBrick)
			vy = -MOVING_SPEED_FORM_BRICK;
		else
			vy = -MOVE_SPEED_UP;
		if (start_y - y > DISTANCE_MOVE)
			isdone = true;		// da xuat hien hieu ung tien va bay len xong roi
		x += dx;
		y += dy;		
	}
	else if(!isdone)
	{
		vx = 0;
		vy = 0;
		CGameObject::Update(dt);
	}
}

void CMonneyEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*left = x;
	top = y;
	right = x + 8;
	bottom = y + 8;*/
}

void CMonneyEffect::Render()
{
	if (state != HIT_EFFECT)
	{
		if (!isdone)
			animation_set->at(state)->Render(x, y);
	}
	else
	{
		if (GetTickCount64() - timeRenderHitEffect < 25)
			animation_set->at(state)->Render(x, y);
		else
			isdone = true;
	}
}

void CMonneyEffect::SetState(int State)
{
	CGameObject::SetState(State);
	switch (State)
	{
	case MAKE_100:
		break;
	case MAKE_200:
		break;
	case MAKE_400:
		break;
	case MAKE_800:
		break;
	case MAKE_1000:
		break;
	case MAKE_2000:
		break;
	case MAKE_4000:
		break;
	case MAKE_8000:
		break;
	case MAKE_ONE_UP:
		break;
	case HIT_EFFECT:
		timeRenderHitEffect = GetTickCount64();
		break;
	}
}




