#include "Coin.h"
#include "MonneyEffect.h"

CCoin::CCoin()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(13));
	//this->SetAnimationSet(CAnimationSets::GetInstance()->Get(8));
}
void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (start_y == 0)
		start_y = y;
	/*if (!isdone)
	{*/
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
				if (!isdone)
					makeEffect = true;
				isdone = true;
							
			}
		}
	
	if (makeEffect)
	{
		CMonneyEffect* monneyeffect = new CMonneyEffect();
		monneyeffect->SetPosition(x, start_y-15);
		monneyeffect->SetState(MAKE_100);
		makeEffect = false;
		listEffect.push_back(monneyeffect);
	}
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Update(dt, coObjects);
	}
	y += dy;
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	/*left = x;
	top = y;
	right = x + 8;
	bottom = y + 8;*/
}

void CCoin::Render()
{
	if(!isdone)
		animation_set->at(1)->Render(x, y);
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Render();
	}
	//RenderBoundingBox();
}



