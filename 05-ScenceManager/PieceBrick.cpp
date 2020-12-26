#include "PieceBrick.h"
CPieceBrick::CPieceBrick(D3DXVECTOR2 position, int nx, int deflectFactorY)
{
	x = position.x;
	y = position.y;
	vx = MOVING_SPEED_X * nx;
	vy = -MOVING_SPEED_Y * deflectFactorY;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(11));
}

void CPieceBrick::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += GRAVITY * dt;

	x += dx;
	y += dy;

	if (y > CGame::GetInstance()->GetCamPosY() + SCREEN_HEIGHT / 2)
		isdone = true;
}

void CPieceBrick::Render()
{
	if(!isdone)
		animation_set->at(0)->Render(x, y);
}

void CPieceBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}