#include "BoomerangBrother.h"
#include "MonneyEffect.h"
#include "Boomerang.h"

CBoomerangBrother::CBoomerangBrother(CMario* mario) : CGameObject()
{
	Mario = mario;
	SetState(BROTHER_STATE_MOVING);
}


void CBoomerangBrother::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);
	if (startX == 0)
		startX = x;
	if (x <= startX)
	{
		x = startX;
		vx = 0.02f;
	}
	if (x > startX + DISTANCE_GO_AROUND)
	{
		x = startX + DISTANCE_GO_AROUND;
		vx = -0.02f;
	}
	y += dy;
	x += dx;
	if (countBoomerang < 2 )
	{
		if (countBoomerang == 0)
		{
			SetState(BROTHER_STATE_READY_TO_FIGHT);
			CBoomerang* boomerang = new CBoomerang(1, Mario, this);
			boomerang->SetPosition(x + 10, y);
			listBoomerang.push_back(boomerang);
			countBoomerang++;
			timereadyToThrow = GetTickCount64();
			//SetState(BROTHER_STATE_MOVING);
		}
		else if (countBoomerang == 1)
		{
			if (GetTickCount64() - timereadyToThrow > 700)
			{
				SetState(BROTHER_STATE_READY_TO_FIGHT);
				CBoomerang* boomerang = new CBoomerang(1, Mario, this);
				boomerang->SetPosition(x + 10, y);
				countBoomerang++;
				listBoomerang.push_back(boomerang);
			}
			else
				SetState(BROTHER_STATE_MOVING);
		}
		//SetState(BROTHER_STATE_MOVING);
	}
	if (GetTickCount64() - timereadyToThrow > 2200 && timereadyToThrow != 0)
	{		
		timereadyToThrow = 0;
	}
	for (int i = 0; i < listBoomerang.size(); i++)
	{
		listBoomerang[i]->Update(dt, coObjects);
		if (i == 1)
		{
			CBoomerang* boomerang = dynamic_cast<CBoomerang*>(listBoomerang[i]);
			if (boomerang->isdone)
			{
				countBoomerang = 0;
				listBoomerang.clear();
			}

		}
	}
	if (makeEffect)
	{
		CMonneyEffect* monneyeffect = new CMonneyEffect();
		monneyeffect->SetPosition(x, y);
		monneyeffect->SetState(MAKE_1000);
		Mario->score += 1000;
		makeEffect = false;
		listEffect.push_back(monneyeffect);
	}
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Update(dt, coObjects);
		if (listEffect[i]->isdone)
		{			
			isdone = true;			
		}
	}
}
void CBoomerangBrother::Render()
{
	if(!isKill)
		animation_set->at(state)->Render(x, y);

	for (int i = 0; i < listBoomerang.size(); i++)
	{
		listBoomerang[i]->Render();
	}
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Render();
	}
	//RenderBoundingBox();
}

void CBoomerangBrother::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BROTHER_STATE_MOVING:
		break;	
	case BROTHER_STATE_READY_TO_FIGHT:
		break;
	}
}

void CBoomerangBrother::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	left = x;
	top = y;
	right = x + BROTHER_BBOX_WIDTH;
	bottom = y + BROTHER_BBOX_HEIGHT;

}


