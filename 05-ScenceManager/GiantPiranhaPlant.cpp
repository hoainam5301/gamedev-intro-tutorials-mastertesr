#include "GiantPiranhaPlant.h"
#include "Utils.h"


CGiantPiranhaPlant::CGiantPiranhaPlant(float x, float y) : CGameObject()
{
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}
void CGiantPiranhaPlant::Render()
{	
	animation_set->at(0)->Render(x, y);
}

void CGiantPiranhaPlant::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GIANT_STATE_OPEN:
		break;
	case GIANT_STATE_CLOSE:
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
void CGiantPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//vx = 0.05f;
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;
	if (x >= 134)
		vx = -0.05;
	else if (x <= 80)
		vx = 0.05;
}

