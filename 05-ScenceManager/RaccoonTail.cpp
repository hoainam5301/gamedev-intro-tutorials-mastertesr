#include "RaccoonTail.h"
#include "BrokenBrick.h"
#include "BoomerangBrother.h"
#include "MonneyEffect.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Brick.h"


CRaccoonTail::CRaccoonTail()
{
	 //Mario = mario;
}

void CRaccoonTail::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{	
	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 
	/*if (hitEffect)
	{
		CMonneyEffect* hiteffect = new CMonneyEffect();
		hiteffect->SetPosition(x+15, y);
		hiteffect->SetState(HIT_EFFECT);
		hitEffect = false;
		listEffect.push_back(hiteffect);		
	}
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Update(dt, coObjects);
	}*/
	CGameObject::Update(dt);
	Collision_ENEMY(coObjects);	
}

void CRaccoonTail::Collision_ENEMY(vector<LPGAMEOBJECT>* coObjects)
{
	float l_ene, t_ene, r_ene, b_ene, l_tail, t_tail, r_tail, b_tail;
	GetBoundingBox(l_tail, t_tail, r_tail, b_tail);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (dynamic_cast<CBrokenBrick*>(e))
		{
			CBrokenBrick* brokenbrick = dynamic_cast<CBrokenBrick*>(e);
			brokenbrick->GetBoundingBox(l_ene, t_ene, r_ene, b_ene);
			if (CGameObject::CheckAABB(l_ene , t_ene , r_ene , b_ene , l_tail, t_tail, r_tail, b_tail))
			{
				brokenbrick->isDestroyed = true;
				brokenbrick->SetState(STATE_DESTROYED);
			}
		}
		else if (dynamic_cast<CBoomerangBrother*>(e))
		{
			CBoomerangBrother* brother = dynamic_cast<CBoomerangBrother*>(e);
			brother->GetBoundingBox(l_ene, t_ene, r_ene, b_ene);
			if (CGameObject::CheckAABB(l_ene, t_ene, r_ene, b_ene, l_tail, t_tail, r_tail, b_tail))
			{
				//brother->makeEffect = true;
				//brother->isKill = true;
			}
		}
		else if (dynamic_cast<CGoomba*>(e))
		{
			CGoomba* goomba = dynamic_cast<CGoomba*>(e);
			goomba->GetBoundingBox(l_ene, t_ene, r_ene, b_ene);
			if (CGameObject::CheckAABB(l_ene, t_ene, r_ene, b_ene, l_tail, t_tail, r_tail, b_tail))
			{				
				goomba->hitByTail = true;				
				if (goomba->id_goomba == GOOMBA_NORMAL)
				{
					goomba->makeEffect = true;
					goomba->hitEffect = true;
					goomba->SetState(GOOMBA_STATE_DIE_FLY);
				}
				else if (goomba->id_goomba == 2)
				{
					goomba->makeEffect = true;
					goomba->hitEffect = true;
					goomba->hasWing = false;
					goomba->SetState(GOOMBA_RED_STATE_NO_WING_DIE_FLY);
				}
			}
		}
		else if (dynamic_cast<CKoopas*>(e))
		{
			CKoopas* koopas = dynamic_cast<CKoopas*>(e);
			koopas->GetBoundingBox(l_ene, t_ene, r_ene, b_ene);
			if (CGameObject::CheckAABB(l_ene, t_ene, r_ene, b_ene, l_tail, t_tail, r_tail, b_tail))
			{
				//koopas->makeEffect =true;
				if (koopas->id_koopa == KOOPA_RED)
				{
					koopas->hitEffect = true;
					koopas->SetState(KOOPA_RED_STATE_DIE_UP);
					
				}
				else if (koopas->id_koopa == KOOPA_GREEN)
				{
					koopas->hitEffect = true;
					koopas->SetState(KOOPA_GREEN_STATE_DIE_UP);
					koopas->hasWing = false;					
				}
				
				koopas->hitByTail = true;
				
			}
		}
		else if (dynamic_cast<CBrick*>(e))
		{
			CBrick* brick = dynamic_cast<CBrick*>(e);
			brick-> GetBoundingBox(l_ene, t_ene, r_ene, b_ene);
			if (CGameObject::CheckAABB(l_ene, t_ene, r_ene, b_ene, l_tail, t_tail, r_tail, b_tail))
			{				
				brick->hitByTail = true;
				//brick->bouncing = 1;
				brick->bottom_coll = 1;	
				hitEffect = true;
			}
		}
	}
}

void CRaccoonTail::Render()
{
	/*for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Render();
	}*/
	//RenderBoundingBox();
}

void CRaccoonTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y ;
	r =	x+ WIDTH_BBOX;
	b = y+ HEIGHT_BBOX;
}
