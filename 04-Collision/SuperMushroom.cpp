#include "SuperMushroom.h"
#include "debug.h"
void CSuperMushroom::Render()
{
	animations[0]->Render(x, y);
	//RenderBoundingBox();
}

void CSuperMushroom::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + SUPERMUSHROOM_BBOX_WIDTH;
	b = y + SUPERMUSHROOM_BBOX_HEIGHT;
}

void CSuperMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure SuperMushroom can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
	/*DebugOut(L"toa do y %f \n", y);
	DebugOut(L"toa do x %f \n", x);
	DebugOut(L"flag %d \n", flag);*/

	if (x < 84 && flag == 0)
	{
		vy = 0.06;
		if (y >= 134)
		{
			vy = 0;
			flag = 1;
		}
	}
	else
	{
		if (vx < 0 && x < 0) {
			x = 0; vx = -vx;
		}
		if (vx > 0 && x > 290) {
			x = 290; vx = -vx;
		}
	}
}

void CSuperMushroom::SetState(int state)
{
	CGameObject::SetState(state);
	if (state == SUPERMUSHROOM_STATE_WALKING)
		vx = -SUPERMUSHROOM_WALKING_SPEED;
}