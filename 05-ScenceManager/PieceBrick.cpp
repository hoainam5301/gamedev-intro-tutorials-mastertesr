#include "PieceBrick.h"
CPieceBrick::CPieceBrick(D3DXVECTOR2 position, int nx, int deflectFactorY)
{
	x = position.x;
	y = position.y;
	vx = 0.07 * nx;
	vy = -0.16 * deflectFactorY;
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(11));
}

void CPieceBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += 0.005 * dt;

	x += dx;
	y += dy;

	/*if (y > CGame::GetInstance()->GetCamPosY() + SCREEN_HEIGHT / 2)
		isFinishedUsing = true;*/
}

void CPieceBrick::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CPieceBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}