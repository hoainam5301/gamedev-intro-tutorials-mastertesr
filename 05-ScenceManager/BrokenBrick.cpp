#include "BrokenBrick.h"

CBrokenBrick::CBrokenBrick(int id_state)
{
	
	id_broken_state = id_state;
	if (this->id_broken_state == 1)
		SetState(STATE_BRICK_NORMAL);
	else if (this->id_broken_state == 2)
		SetState(STATE_COIN_ROTATE);
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(12));
	//DebugOut(L"Gia tri cua state %d \n", id_broken_state);

}

void CBrokenBrick::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"Gia tri cua state %d \n", id_broken_state);
	/*if (isDestroyed)
		DebugOut(L"dasdasdasdasda");*/
	
	for (int i=0;i<listPiece.size();i++)
	{
		listPiece[i]->Update(dt, coObjects);	
	}
	
	if (tranformation && GetTickCount64()-timeTranformation<10000 && timeTranformation!=0 && !hasTranformation)
	{
		//DebugOut(L"aaaaaaaaaaaaa");
		if (GetState() == STATE_BRICK_NORMAL)
			SetState(STATE_COIN_NO_ROTATE);
		else if (GetState() == STATE_COIN_ROTATE)
			SetState(STATE_BRICK_NORMAL);
		hasTranformation = true;
	}
	if(GetTickCount64()-timeTranformation>10000 && timeTranformation!=0)
	{		
		tranformation = false;
		timeTranformation = 0;
		if (GetState() == STATE_BRICK_NORMAL)
			SetState(STATE_COIN_NO_ROTATE);
		else if (GetState() == STATE_COIN_ROTATE)
			SetState(STATE_BRICK_NORMAL);
		else if (GetState() == STATE_COIN_NO_ROTATE)
			SetState(STATE_BRICK_NORMAL);
	}
	//DebugOut(L"time tran %d \n", timeTranformation);
}

void CBrokenBrick::Render()
{
	//RenderBoundingBox();
	if(!isDestroyed)
		animation_set->at(state)->Render(x,y);
	//DebugOut(L"Gia tri cua state %d \n", state);
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
	/*if (State == STATE_BRICK_NORMAL)
	{
		
	}
	else if (State == STATE_COIN_NO_ROTATE)
	{
		
	}*/
	
}