#include "BrokenBrick.h"



CBrokenBrick::CBrokenBrick()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(12));
}

void CBrokenBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	/*if (isDestroyed)
		DebugOut(L"dasdasdasdasda");*/
	for (LPGAMEOBJECT piece : listPiece)
	{
		piece->Update(dt, coObjects);	
	}
}

void CBrokenBrick::Render()
{
	RenderBoundingBox();
	if(!isDestroyed)
		animation_set->at(0)->Render(x,y);
	for (LPGAMEOBJECT piece : listPiece)
	{
		piece->Render();
	}
}
void CBrokenBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isDestroyed)
		return;
	l = x;
	t = y;
	r = x + 16;
	b = y + 16;
}
void CBrokenBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STATE_DESTROYED:
		//isDestroyed = true;
		CPieceBrick* topLeftPiece = new CPieceBrick({ x - 1, y - 2 }, -1, 2);
		CPieceBrick* topRightPiece = new CPieceBrick({ x + 9, y - 2 }, 1, 2);
		CPieceBrick* bottomLeftPiece = new CPieceBrick({ x - 1, y + 8 }, -1);
		CPieceBrick* bottomRightPiece = new CPieceBrick({ x + 9, y +8 }, 1);
		listPiece = { topLeftPiece, topRightPiece, bottomLeftPiece, bottomRightPiece };
		break;
	}
	
}