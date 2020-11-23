#include "Coin.h"

CCoin::CCoin()
{
	//this->SetAnimationSet(CAnimationSets::GetInstance()->Get(13));
}
void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vx = 0;
	vy = 0;	
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*left = x;
	top = y;
	right = x + 8;
	bottom = y + 8;*/
}

void CCoin::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}



