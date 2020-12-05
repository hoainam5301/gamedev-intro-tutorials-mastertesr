#include "MonneyEffect.h"


CMonneyEffect::CMonneyEffect()
{ 
	
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(15));
	
}
void CMonneyEffect::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isdone)
	{
		CGameObject::Update(dt);
		if (start_y == 0)
			start_y = y;
		//DebugOut(L"gia tri start y %f \n", start_y);
		//DebugOut(L"gia tri y %f \n", y);
		vy = -0.1;
		if (start_y - y > 30)
			isdone = true;		// da xuat hien hieu ung tien va bay len xong roi
		x += dx;
		y += dy;
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
	if(!isdone)
		animation_set->at(state)->Render(x, y);	
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
	}
}




