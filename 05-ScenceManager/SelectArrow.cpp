#include "SelectArrow.h"

CSelectArrow::CSelectArrow(CMario* mario)
{
	Mario = mario;
}
void CSelectArrow::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (startX == 0)
		startX = x;
	if (startY == 0)
		startY = y;
	if (Mario->isAutoGoDown)
		SetPosition(80, 160);
	else if (Mario->isAutoGoTop)
		SetPosition(80, 145);
	CGameObject::Update(dt,coObjects);
	//DebugOut(L"aaaa %d \n",tranformation);
	
}
void CSelectArrow::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	
}

void CSelectArrow::Render()
{
	int ani=0;
	animation_set->at(ani)->Render(x, y);
}

