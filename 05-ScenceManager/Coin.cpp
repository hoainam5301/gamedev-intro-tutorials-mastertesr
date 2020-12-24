#include "Coin.h"
#include "MonneyEffect.h"

CCoin::CCoin()
{
	SetState(COIN_STATE_ROTATE);
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(13));
}
void CCoin::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt,coObjects);
	//DebugOut(L"aaaa %d \n",tranformation);
	if (isdone )
		return;
	if (start_y == 0)
		start_y = y;
	/*if (!isdone)
	{*/if (GetState() == COIN_STATE_CREATED)
	{
		if (spawn)
		{
			if (y >= start_y - 60)
				vy = -0.0025;
			else spawn = false;
		}
		else
		{
			if (start_y - y > 20)
			{
				vy = 0.0025;

			}
			else
			{
				vy = 0;
				y = start_y - 20;
				if (!hasCreatMoneyEffect)
				{
					makeEffect = true;
					hasCreatMoneyEffect = true;
				}	//da xong cac viec di chuyen cua coin va hieu ung monneyeffect	

			}
		}

		if (makeEffect)
		{
			CMonneyEffect* monneyeffect = new CMonneyEffect();
			monneyeffect->SetPosition(x, start_y - 15);
			monneyeffect->SetState(MAKE_100);
			monneyeffect->coinFormBrick = true;
			makeEffect = false;
			listEffect.push_back(monneyeffect);
		}
		/*for (int i = 0; i < listEffect.size(); i++)
		{
			listEffect[i]->Update(dt);
			if (listEffect[i]->isdone)
			{
				isdone = true;
				DebugOut(L"aaaa");
			}
		}*/

		y += dy;
	}
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (tranformation)
	{
		left = top = right = bottom = 0;
	}
	else if (!isdone )
	{
		left = x;
		top = y;
		right = x + 16;
		bottom = y + 16;
	}
	
}

void CCoin::Render()
{
	if (tranformation)
		return;
	else if(!hasCreatMoneyEffect && !isdone)
		animation_set->at(state)->Render(x, y);
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Render();
	}
	RenderBoundingBox();
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_CREATED:
		break;
	case COIN_STATE_NO_ROTATE:
		break;
	case COIN_STATE_ROTATE:
		break;
	}
}



