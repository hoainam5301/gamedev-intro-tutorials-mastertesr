#include "BrokenBrick.h"

CBrokenBrick::CBrokenBrick(int id_state)
{
	
	id_broken_state = id_state;
	if (this->id_broken_state == STATE_BRICK_NORMAL)
		SetState(STATE_BRICK_NORMAL);
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(12));
}

void CBrokenBrick::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	for (int i=0;i<listPiece.size();i++)
	{		
		listPiece[i]->Update(dt, coObjects);	
		if (listPiece[i]->isdone)
			isdone = true;
	}	
}

void CBrokenBrick::Render()
{
	//RenderBoundingBox();
	if (!isDestroyed && !tranformation)
		animation_set->at(state)->Render(x, y);
	for (LPGAMEOBJECT piece : listPiece)
	{
		piece->Render();
	}
}
void CBrokenBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isDestroyed || tranformation)
		return;
	l = x;
	t = y;
	r = l + WIDTH_HEIGHT_BROKENBRICK;
	b = t + WIDTH_HEIGHT_BROKENBRICK;
}
void CBrokenBrick::SetState(int State)
{
	CGameObject::SetState(State);
	switch (State)
	{
	case STATE_BRICK_NORMAL:
		break;
	case STATE_COIN_NO_ROTATE:
		break;
	case STATE_COIN_ROTATE:
		break;
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