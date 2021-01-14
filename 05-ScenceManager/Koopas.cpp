#include "Koopas.h"
#include "ColorBox.h"
#include "Floor.h"
#include "MonneyEffect.h"
#include "BrokenBrick.h"
#include "Brick.h"
#include "Goomba.h"
#include "GiantPiranhaPlant.h"
#include "GiantPiranhaPlantBite.h"


CKoopas::CKoopas(CMario* mario,int id_Koopa,int haswing)
{
	Mario = mario;
	id_koopa = id_Koopa;
	hasWing = haswing;
	if (id_koopa == KOOPA_RED)
	{
		if(hasWing)
			SetState(KOOPA_RED_STATE_HAS_WING_WALKING_LEFT);
		else 
			SetState(KOOPA_RED_STATE_WALKING_LEFT);
	}
	else if (id_koopa == KOOPA_GREEN)
	{
		if (hasWing)
			SetState(KOOPA_GREEN_STATE_HAS_WING_FLY_LEFT);
		else
			SetState(KOOPA_GREEN_STATE_WALKING_LEFT);
		timeToFly = GetTickCount64();		
	}
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x+1;
	top = y+13;
	bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	right = x + KOOPAS_BBOX_WIDTH;
	if (id_koopa == KOOPA_RED)
	{
		if (state == KOOPA_RED_STATE_REVIVE || state == KOOPA_RED_STATE_REVIVE_UP)
		{
			left = x;
			bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
		}
		else if (state == KOOPA_RED_STATE_WALKING_RIGHT || state == KOOPA_RED_STATE_WALKING_LEFT || state == KOOPA_RED_STATE_HAS_WING_WALKING_LEFT )
		{
			top = y;
			bottom = top + KOOPAS_BBOX_HEIGHT;
		}
		else if (state == KOOPA_RED_STATE_DIE_UP)
		{
			/*if(hitByWeapon)
				left = top = right = bottom = 0;	*/	
		}
	}
	else if (id_koopa == KOOPA_GREEN)
	{
		if (state == KOOPA_GREEN_STATE_REVIVE || state == KOOPA_GREEN_STATE_REVIVE_UP)
		{		
			left = x;
			bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
		}
		else if (state == KOOPA_GREEN_STATE_WALKING_RIGHT || state == KOOPA_GREEN_STATE_WALKING_LEFT || state==KOOPA_GREEN_STATE_HAS_WING_FLY_LEFT || state==KOOPA_GREEN_STATE_HAS_WING_FLY_RIGHT)
		{
			top = y;
			bottom = top + KOOPAS_BBOX_HEIGHT;		
		}
		else if (state == KOOPA_GREEN_STATE_DIE_UP)
		{			
			/*if (hitByWeapon)
				left = top = right = bottom = 0;	*/	
		}
	}
}

void CKoopas::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (startX == 0)
		startX = x;
	if (startY == 0)
		startY = y;
	if (GetState() == KOOPA_RED_STATE_HOLDING || 
		GetState() == KOOPA_RED_STATE_HOLDING_UP || 
		GetState() == KOOPA_GREEN_STATE_HOLDING || 
		GetState() == KOOPA_GREEN_STATE_HOLDING_UP)
	{
		if (Mario->level == MARIO_RACCOON)
		{
			if (Mario->nx > 0)
				SetPosition(Mario->x + 17, Mario->y);
			else
				SetPosition(Mario->x - 12, Mario->y);
		}
		else if (Mario->level == MARIO_LEVEL_SMALL)
		{
			if (Mario->nx > 0)
				SetPosition(Mario->x + 19, Mario->y + 2);
			else
				SetPosition(Mario->x - 1, Mario->y + 2);
		}
		else
		{
			if (Mario->nx > 0)
				SetPosition(Mario->x + 20, Mario->y);
			else
				SetPosition(Mario->x - 6, Mario->y);
		}
	}
	if (id_koopa == KOOPA_RED)
	{
		if (hasWing)
		{
			vx = 0;
			if(y==startY)
				vy = 0.05;
			if (y > startY + 50)
				vy = -0.05;
			if (y < startY)
			{
				vy = 0;
				y = startY;
			}
		}
		if (!Mario->isHolding && (last_state == KOOPA_RED_STATE_HOLDING || last_state == KOOPA_RED_STATE_HOLDING_UP))//de khi tha mai rua ra thi mai rua bi da
		{
			nx = Mario->nx;
			if (last_state == KOOPA_RED_STATE_HOLDING)
				SetState(KOOPA_RED_STATE_DIE_AND_MOVE);
			else if (last_state == KOOPA_RED_STATE_HOLDING_UP)
				SetState(KOOPA_RED_STATE_DIE_AND_MOVE_UP);

		}
		if (GetTickCount64() - timeToRevive > TIME_TO_READY_REVIVE && (last_state == KOOPA_RED_STATE_DIE || last_state == KOOPA_RED_STATE_DIE_UP || last_state == KOOPA_RED_STATE_HOLDING || last_state == KOOPA_RED_STATE_HOLDING_UP))//koopas vao trang thai chuan bi hoi sinh
		{
			Mario->isHolding = false;
			if (last_state == KOOPA_RED_STATE_DIE || last_state == KOOPA_RED_STATE_HOLDING)
				SetState(KOOPA_RED_STATE_REVIVE);
			else if (last_state == KOOPA_RED_STATE_DIE_UP || last_state == KOOPA_RED_STATE_HOLDING_UP)
				SetState(KOOPA_RED_STATE_REVIVE_UP);
			hitByTail = false;
		}
		if (GetTickCount64() - timeToRevive > TIME_REVIVE && (last_state == KOOPA_RED_STATE_REVIVE || last_state == KOOPA_RED_STATE_REVIVE_UP))
		{
			SetState(KOOPA_RED_STATE_WALKING_RIGHT);
			timeToRevive = 0;
			Mario->isHolding = false;
		}
	}
	else if (id_koopa == KOOPA_GREEN)
	{
		if (hasWing)
		{
			if (GetTickCount64() - timeToFly > TIME_READY_FLY && !hitByWeapon)
			{
				SetState(KOOPA_GREEN_STATE_HAS_WING_FLY_LEFT);
				timeToFly = GetTickCount64();
			}
		}
		else
		{
			if (!Mario->isHolding && (last_state == KOOPA_GREEN_STATE_HOLDING || last_state == KOOPA_GREEN_STATE_HOLDING_UP))//de khi tha mai rua ra thi mai rua bi da
			{
				nx = Mario->nx;
				if (last_state == KOOPA_GREEN_STATE_HOLDING)
					SetState(KOOPA_GREEN_STATE_DIE_AND_MOVE);
				else if (last_state == KOOPA_GREEN_STATE_HOLDING_UP)
					SetState(KOOPA_GREEN_STATE_DIE_AND_MOVE_UP);
				hitByTail = false;
			}
			if (GetTickCount64() - timeToRevive > TIME_TO_READY_REVIVE && (last_state == KOOPA_GREEN_STATE_DIE || last_state == KOOPA_GREEN_STATE_DIE_UP || last_state == KOOPA_GREEN_STATE_HOLDING || last_state == KOOPA_GREEN_STATE_HOLDING_UP))//koopas vao trang thai chuan bi hoi sinh
			{
				Mario->isHolding = false;
				if (last_state == KOOPA_GREEN_STATE_DIE || last_state == KOOPA_GREEN_STATE_HOLDING)
					SetState(KOOPA_GREEN_STATE_REVIVE);
				else if (last_state == KOOPA_GREEN_STATE_DIE_UP || last_state == KOOPA_GREEN_STATE_HOLDING_UP)
					SetState(KOOPA_GREEN_STATE_REVIVE_UP);
			}
			if (GetTickCount64() - timeToRevive > TIME_REVIVE && (last_state == KOOPA_GREEN_STATE_REVIVE || last_state == KOOPA_GREEN_STATE_REVIVE_UP))
			{
				if (vx < 0)
					SetState(KOOPA_GREEN_STATE_WALKING_LEFT);
				else
					SetState(KOOPA_GREEN_STATE_WALKING_RIGHT);
				timeToRevive = 0;
			}
		}
	}
	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 
	if (makeEffect)
	{
		CMonneyEffect* monneyeffect = new CMonneyEffect();
		monneyeffect->SetPosition(x, y);
		monneyeffect->SetState(MAKE_100);
		Mario->score += 100;
		makeEffect = false;
		listEffect.push_back(monneyeffect);
	}

	CGameObject::Update(dt);
	if ((id_koopa == KOOPA_RED && !hasWing) || id_koopa==KOOPA_GREEN )
	{
		if (state != KOOPA_RED_STATE_HOLDING &&
			state != KOOPA_RED_STATE_HOLDING_UP &&
			state != KOOPA_GREEN_STATE_HOLDING &&
			state != KOOPA_GREEN_STATE_HOLDING_UP)
			vy += GARVITY * dt;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		x += min_tx * dx + nx * 0.5f;
		y += min_ty * dy + ny * 0.01f;

		if (ny != 0)
			vy = 0;
		if (ny != 0 && (state == KOOPA_RED_STATE_DIE_UP || state==KOOPA_RED_STATE_REVIVE_UP || state==KOOPA_GREEN_STATE_REVIVE_UP || state==KOOPA_GREEN_STATE_DIE_UP))
		{
			vx = 0;
		}
	
		//Collision logic with other objects

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (id_koopa == KOOPA_RED)
			{
				if (dynamic_cast<CColorBox*>(e->obj))
				{
					CColorBox* colorbox = dynamic_cast<CColorBox*>(e->obj);
					if (e->nx != 0)
					{
						x += dx*2;
					}
					else if (e->ny < 0 && GetState()!=KOOPA_RED_STATE_DIE_AND_MOVE && GetState()!=KOOPA_RED_STATE_DIE_AND_MOVE_UP && GetState()!=KOOPA_RED_STATE_DIE&& GetState()!=KOOPA_RED_STATE_DIE_UP )
					{
						if (!hitByWeapon)
						{
							if (x <= colorbox->x)
							{
								x = colorbox->x;
								SetState(KOOPA_RED_STATE_WALKING_RIGHT);

							}
							else if (x >= colorbox->x + KOOPAS_BBOX_WIDTH * colorbox->width - KOOPAS_BBOX_WIDTH)
							{
								x = colorbox->x + KOOPAS_BBOX_WIDTH * colorbox->width - KOOPAS_BBOX_WIDTH;
								SetState(KOOPA_RED_STATE_WALKING_LEFT);
							}
						}
					}

				}
				else if (dynamic_cast<CGoomba*>(e->obj))
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					if (e->nx != 0)
					{
						if (GetState() == KOOPA_RED_STATE_DIE_AND_MOVE || GetState() == KOOPA_RED_STATE_DIE_AND_MOVE_UP)
						{
							if (goomba->id_goomba == GOOMBA_NORMAL)
							{		
								goomba->hitByWeapon = true;
								goomba->SetState(GOOMBA_STATE_DIE_FLY);
							}
							else if (goomba->id_goomba == GOOMBA_RED)
							{
								goomba->hasWing = false;
								goomba->hitByWeapon = true;
								goomba->SetState(GOOMBA_RED_STATE_NO_WING_DIE_FLY);
							}
						}
						else
						{
							goomba->vx = -goomba->vx;
							if (goomba->id_goomba == GOOMBA_NORMAL && !goomba->hitByWeapon)
								goomba->SetState(GOOMBA_STATE_WALKING);
							else if (goomba->id_goomba == GOOMBA_RED)
							{
								if (goomba->hasWing)
								{
									goomba->SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
									//DebugOut(L"gia tri thoi gian %d \n", GetTickCount64());	
									if (goomba->sulkyMario == 0)
										goomba->sulkyMario = GetTickCount64();
									if (GetTickCount64() - goomba->sulkyMario < TIME_READY_TO_SULKY)
									{
										if (Mario->x - this->x > 0)
											this->nx = 1;
										else
											this->nx = -1;
										goomba->SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
									}
									else
									{
										goomba->sulkyMario = 0;
										goomba->SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
									}
								}
								else
									goomba->SetState(GOOMBA_RED_STATE_NO_WING_WALK);							
							}
						}
					}
				}
				else if (dynamic_cast<CKoopas*>(e->obj))
				{
					CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
					if (GetState() == KOOPA_RED_STATE_DIE_AND_MOVE || GetState() == KOOPA_RED_STATE_DIE_AND_MOVE_UP)
					{
						if (koopas->id_koopa == KOOPA_RED)
						{
							koopas->hasWing = false;
							koopas->hitByWeapon = true;
							koopas->SetState(KOOPA_RED_STATE_DIE_UP);
						}
						else if (koopas->id_koopa == KOOPA_GREEN)
						{
							koopas->hasWing = false;
							koopas->hitByWeapon = true;
							koopas->SetState(KOOPA_GREEN_STATE_DIE_UP);
						}
					}
					else
					{
						if (GetState() == KOOPA_RED_STATE_WALKING_RIGHT)
							SetState(KOOPA_RED_STATE_WALKING_LEFT);
						else if (GetState() == KOOPA_RED_STATE_WALKING_LEFT)
							SetState(KOOPA_RED_STATE_WALKING_RIGHT);
						if (koopas->id_koopa == KOOPA_RED)
						{
							if (koopas->GetState() == KOOPA_RED_STATE_WALKING_RIGHT)
								koopas->SetState(KOOPA_RED_STATE_WALKING_LEFT);
							else if (koopas->GetState() == KOOPA_RED_STATE_WALKING_LEFT)
								koopas->SetState(KOOPA_RED_STATE_WALKING_RIGHT);
						}
						else if (koopas->id_koopa == KOOPA_GREEN)
						{
							if (!koopas->hasWing)
							{
								if (koopas->GetState() == KOOPA_GREEN_STATE_WALKING_RIGHT)
									koopas->SetState(KOOPA_GREEN_STATE_WALKING_LEFT);
								else if (koopas->GetState() == KOOPA_GREEN_STATE_WALKING_LEFT)
									koopas->SetState(KOOPA_GREEN_STATE_WALKING_RIGHT);
							}
						}
					}
				}
				else if (dynamic_cast<CGiantPiranhaPlant*>(e->obj))
				{
					CGiantPiranhaPlant* plant = dynamic_cast<CGiantPiranhaPlant*>(e->obj);
					if (e->nx != 0)
					{
						plant->isdone = true;
					}
				}
				else if (dynamic_cast<CGiantPiranhaPlantBite*>(e->obj))
				{
					CGiantPiranhaPlantBite* plantbite = dynamic_cast<CGiantPiranhaPlantBite*>(e->obj);
					if (e->nx != 0)
					{
						plantbite->isdone = true;
					}
				}
				else if (dynamic_cast<CBrokenBrick*>(e->obj))
				{
					CBrokenBrick* brokenbrick = dynamic_cast<CBrokenBrick*>(e->obj);
					if (GetState() == KOOPA_RED_STATE_DIE_AND_MOVE || GetState() == KOOPA_RED_STATE_DIE_AND_MOVE_UP)
					{						
						brokenbrick->isDestroyed = true;
						brokenbrick->SetState(STATE_DESTROYED);
						vx = -vx;
					}
					else if (e->ny < 0 && GetState() != KOOPA_RED_STATE_DIE_AND_MOVE && GetState() != KOOPA_RED_STATE_DIE_AND_MOVE_UP)
					{
						if (x <= brokenbrick->x)
						{
							x = brokenbrick->x;
							SetState(KOOPA_RED_STATE_WALKING_RIGHT);

						}
						else if (x >= brokenbrick->x + KOOPAS_BBOX_WIDTH+10  - KOOPAS_BBOX_WIDTH)
						{
							x = brokenbrick->x + KOOPAS_BBOX_WIDTH +10 - KOOPAS_BBOX_WIDTH;
							SetState(KOOPA_RED_STATE_WALKING_LEFT);
						}
					}
				}
				else if (dynamic_cast<CBrick*>(e->obj))
				{
					if (e->nx != 0)
					{
						vx = -vx;
						if (GetState() == KOOPA_RED_STATE_DIE_AND_MOVE || GetState() == KOOPA_RED_STATE_DIE_AND_MOVE_UP)
						{
							CBrick* brick = dynamic_cast<CBrick*>(e->obj);
							brick->collWithKoopa = true;
						}
					}
				}
				else if (!dynamic_cast<CMario*>(e->obj))
				{
					if (e->nx != 0)
					{
						vx = -vx;
						if (GetState() != KOOPA_RED_STATE_DIE_AND_MOVE && GetState() != KOOPA_RED_STATE_DIE_AND_MOVE_UP && GetState() != KOOPA_RED_STATE_DIE_UP && GetState()!=KOOPA_RED_STATE_DIE)
						{
							if (vx > 0)
								SetState(KOOPA_RED_STATE_WALKING_RIGHT);
							else
								SetState(KOOPA_RED_STATE_WALKING_LEFT);
						}
					}
				}	
				
			}
			else if (id_koopa == KOOPA_GREEN)
			{
				if (dynamic_cast<CColorBox*>(e->obj))
				{
					if (e->nx != 0)
					{
						x += dx*2;
					}						
				}
				else if (dynamic_cast<CGoomba*>(e->obj))
				{
					CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
					if (e->nx != 0)
					{
						if (GetState() == KOOPA_GREEN_STATE_DIE_AND_MOVE || GetState() == KOOPA_GREEN_STATE_DIE_AND_MOVE_UP)
						{
							if (goomba->id_goomba == GOOMBA_NORMAL)
							{
								goomba->SetState(GOOMBA_STATE_DIE_FLY);
							}
							else if (goomba->id_goomba == GOOMBA_RED)
							{
								goomba->hasWing = false;
								goomba->SetState(GOOMBA_RED_STATE_NO_WING_DIE_FLY);
							}
						}
						else
						{
							goomba->vx = -goomba->vx;
							if (goomba->id_goomba == GOOMBA_NORMAL)
								goomba->SetState(GOOMBA_STATE_WALKING);
							else if (goomba->id_goomba == GOOMBA_RED)
							{
								if (goomba->hasWing)
								{
									goomba->SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
									//DebugOut(L"gia tri thoi gian %d \n", GetTickCount64());	
									if (goomba->sulkyMario == 0)
										goomba->sulkyMario = GetTickCount64();
									if (GetTickCount64() - goomba->sulkyMario < TIME_READY_TO_SULKY)
									{
										if (Mario->x - this->x > 0)
											this->nx = 1;
										else
											this->nx = -1;
										goomba->SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
									}
									else
									{
										goomba->sulkyMario = 0;
										goomba->SetState(GOOMBA_RED_STATE_HAS_WING_WALK);
									}
								}
								else
									goomba->SetState(GOOMBA_RED_STATE_NO_WING_WALK);
							}
						}
					}
					else if (e->ny > 0)
					{
						if (goomba->id_goomba == GOOMBA_RED)
							y -= dy * 2;
					}
				}
				else if (dynamic_cast<CKoopas*>(e->obj))
				{
					CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
					if (GetState() == KOOPA_GREEN_STATE_DIE_AND_MOVE || GetState() == KOOPA_GREEN_STATE_DIE_AND_MOVE_UP)
					{
						if (koopas->id_koopa == KOOPA_RED)
						{
							koopas->hasWing = false;
							koopas->hitByWeapon = true;
							koopas->SetState(KOOPA_RED_STATE_DIE_UP);
						}
						else if (koopas->id_koopa == KOOPA_GREEN)
						{
							koopas->hasWing = false;
							koopas->hitByWeapon = true;
							koopas->SetState(KOOPA_GREEN_STATE_DIE_UP);
						}
					}
					else if ((GetState() == KOOPA_GREEN_STATE_HAS_WING_FLY_LEFT || GetState() == KOOPA_GREEN_STATE_HAS_WING_FLY_RIGHT) && e->ny != 0)
					{
						koopas->y-=1;
					}
					else
					{
						if (GetState() == KOOPA_GREEN_STATE_WALKING_RIGHT)
							SetState(KOOPA_GREEN_STATE_WALKING_LEFT);
						else if (GetState() == KOOPA_GREEN_STATE_WALKING_LEFT)
							SetState(KOOPA_GREEN_STATE_WALKING_RIGHT);
						if (koopas->id_koopa == KOOPA_RED)
						{
							if (koopas->GetState() == KOOPA_RED_STATE_WALKING_RIGHT)
								koopas->SetState(KOOPA_RED_STATE_WALKING_LEFT);
							else if (koopas->GetState() == KOOPA_RED_STATE_WALKING_LEFT)
								koopas->SetState(KOOPA_RED_STATE_WALKING_RIGHT);
						}
						else if (koopas->id_koopa == KOOPA_GREEN)
						{
							if (!koopas->hasWing)
							{
								if (koopas->GetState() == KOOPA_GREEN_STATE_WALKING_RIGHT)
									koopas->SetState(KOOPA_GREEN_STATE_WALKING_LEFT);
								else if (koopas->GetState() == KOOPA_GREEN_STATE_WALKING_LEFT)
									koopas->SetState(KOOPA_GREEN_STATE_WALKING_RIGHT);
							}
						}
					}
				}
				else if (dynamic_cast<CGiantPiranhaPlant*>(e->obj))
				{
					CGiantPiranhaPlant* plant = dynamic_cast<CGiantPiranhaPlant*>(e->obj);
					if (e->nx != 0)
					{
						plant->isdone = true;
					}
				}
				else if (dynamic_cast<CGiantPiranhaPlantBite*>(e->obj))
				{
					CGiantPiranhaPlantBite* plantbite = dynamic_cast<CGiantPiranhaPlantBite*>(e->obj);
					if (e->nx != 0)
					{
						plantbite->isdone = true;
					}
				}
				else if (dynamic_cast<CBrokenBrick*>(e->obj))
				{
					if (GetState() == KOOPA_GREEN_STATE_DIE_AND_MOVE || GetState() == KOOPA_GREEN_STATE_DIE_AND_MOVE_UP)
					{
						CBrokenBrick* brokenbrick = dynamic_cast<CBrokenBrick*>(e->obj);
						brokenbrick->isDestroyed=true;
						brokenbrick->SetState(STATE_DESTROYED);						
						vx = -vx;
					}
				}
				else if(!dynamic_cast<CMario*>(e->obj))
				{
					if (!hasWing)
					{
						if (e->nx != 0)
						{
							vx = -vx;
							if (GetState() != KOOPA_GREEN_STATE_DIE_AND_MOVE && GetState() != KOOPA_GREEN_STATE_DIE_AND_MOVE_UP && GetState() != KOOPA_GREEN_STATE_DIE_UP && GetState()!=KOOPA_GREEN_STATE_DIE)
							{
								if (vx > 0)
									SetState(KOOPA_GREEN_STATE_WALKING_RIGHT);
								else
									SetState(KOOPA_GREEN_STATE_WALKING_LEFT);
							}
						}
					}
				}
			}
		}
		
	}
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Update(dt, coObjects);
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	CheckCollisionWithBrick(coObjects);
}

void CKoopas::CheckCollisionWithBrick(vector<LPGAMEOBJECT>* coObjects)
{	
	float l_brick, t_brick, r_brick, b_brick, l_koopa, t_koopa, r_koopa, b_koopa;
	GetBoundingBox(l_koopa, t_koopa, r_koopa, b_koopa);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT e = coObjects->at(i);
		if (dynamic_cast<CBrick*>(e) && GetState()==KOOPA_GREEN_STATE_DIE_AND_MOVE)
		{
			CBrick* brick = dynamic_cast<CBrick*>(e);
			brick->GetBoundingBox(l_brick, t_brick, r_brick, b_brick);
			if (CGameObject::CheckAABB(l_brick, t_brick, r_brick, b_brick, l_koopa, t_koopa, r_koopa, b_koopa))
			{				
				SetState(KOOPA_GREEN_STATE_DIE_UP);
				hitByWeapon = true;
			}
		}
	}
}

void CKoopas::Render()
{
	//DebugOut(L"gia tri state %d \n", state);
	if (hitByWeapon)
	{
		if (GetTickCount64() - timeRenderAniDie > 300 && timeRenderAniDie!=0)
			isdone = true;
		else 
			animation_set->at(state)->Render(x, y);
	}
	else
		animation_set->at(state)->Render(x, y);
	for (int i = 0; i < listEffect.size(); i++)
	{
		listEffect[i]->Render();
	}
	
	RenderBoundingBox();
}

void CKoopas::SetState(int State)
{
	CGameObject::SetState(State);   // last_state de xac dinh trang thai truoc do cua koopas roi thuc hien cac truong hop
	switch (State)
	{
	case KOOPA_RED_STATE_DIE:
		vx = 0;
		vy = 0;
		timeToRevive = GetTickCount64();
		last_state = KOOPA_RED_STATE_DIE;
		break;
	case KOOPA_RED_STATE_DIE_UP:
		if (Mario->nx > 0)
			vx = 0.05;
		else
			vx = -0.05;
		if (last_state != KOOPA_RED_STATE_DIE_AND_MOVE_UP)
			vy = -0.05;		
		if (hitByTail)
		{
			DebugOut(L"aaaaaaa \n");
			
			
			vy = -KOOPAS_MOVING_SPEED*2;
			hitByTail = false;
			makeEffect = true;
		}
		if (hitByWeapon)
		{
			timeRenderAniDie = GetTickCount64();
			vy = -(KOOPAS_MOVING_SPEED * 2);
		}
		timeToRevive = GetTickCount64();
		last_state = KOOPA_RED_STATE_DIE_UP;
		break;
	case KOOPA_RED_STATE_WALKING_RIGHT:
		/*if (last_state == KOOPA_RED_STATE_REVIVE || last_state == KOOPA_RED_STATE_REVIVE_UP)
			y -= KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE;*/
		vx = KOOPAS_WALKING_SPEED;
		last_state = KOOPA_RED_STATE_WALKING_RIGHT;
		break;
	case KOOPA_RED_STATE_WALKING_LEFT:		
		vx = -KOOPAS_WALKING_SPEED;
		last_state = KOOPA_RED_STATE_WALKING_LEFT;
		break;
	case KOOPA_RED_STATE_DIE_AND_MOVE:	
		hitByTail = false;
		vx = KOOPAS_MOVING_SPEED * nx;
		last_state = KOOPA_RED_STATE_DIE_AND_MOVE;
		break;	
	case KOOPA_RED_STATE_REVIVE:
		last_state = KOOPA_RED_STATE_REVIVE;
		break;
	case KOOPA_RED_STATE_REVIVE_UP:
		last_state = KOOPA_RED_STATE_REVIVE_UP;
		break;
	case KOOPA_RED_STATE_DIE_AND_MOVE_UP:
		hitByTail = false;
		last_state = KOOPA_RED_STATE_DIE_AND_MOVE_UP;
		vx = KOOPAS_MOVING_SPEED * nx;
		break;
	case KOOPA_RED_STATE_HOLDING:
		last_state = KOOPA_RED_STATE_HOLDING;
		break;
	case KOOPA_RED_STATE_HOLDING_UP:
		last_state = KOOPA_RED_STATE_HOLDING_UP;
		break;
	case KOOPA_GREEN_STATE_WALKING_RIGHT:
		vx = KOOPAS_WALKING_SPEED;
		last_state = KOOPA_GREEN_STATE_WALKING_RIGHT;
		break;
	case KOOPA_GREEN_STATE_WALKING_LEFT:
		vx = -KOOPAS_WALKING_SPEED;
		last_state = KOOPA_GREEN_STATE_WALKING_LEFT;
		break;
	case KOOPA_GREEN_STATE_DIE:
		vx = 0;
		vy = 0;
		timeToRevive = GetTickCount64();
		last_state = KOOPA_GREEN_STATE_DIE;
		break;
	case KOOPA_GREEN_STATE_DIE_AND_MOVE:
		hitByTail = false;
		last_state = KOOPA_GREEN_STATE_DIE_AND_MOVE;
		vx = 0.1 * nx;
		break;
	case KOOPA_GREEN_STATE_DIE_UP:
		if (Mario->nx > 0)
			vx = KOOPAS_MOVING_SPEED;
		else
			vx = -KOOPAS_MOVING_SPEED;
		if(last_state!=KOOPA_GREEN_STATE_DIE_AND_MOVE_UP)
			vy = -KOOPAS_MOVING_SPEED;
		if (hitByTail )
		{
			makeEffect = true;
			hitByTail = false;
			vy = -KOOPAS_MOVING_SPEED*2;
		}
		if (hitByWeapon)
		{
			timeRenderAniDie = GetTickCount64();
			vy = -KOOPAS_MOVING_SPEED*2;
		}
		timeToRevive = GetTickCount64();
		last_state = KOOPA_GREEN_STATE_DIE_UP;
		break;
	case KOOPA_GREEN_STATE_DIE_AND_MOVE_UP:
		hitByTail = false;
		last_state = KOOPA_GREEN_STATE_DIE_AND_MOVE_UP;
		vx = KOOPAS_MOVING_SPEED * nx;
		break;
	case KOOPA_GREEN_STATE_REVIVE:
		last_state = KOOPA_GREEN_STATE_REVIVE;
		break;
	case KOOPA_GREEN_STATE_REVIVE_UP:
		last_state = KOOPA_GREEN_STATE_REVIVE_UP;
		break;
	case KOOPA_GREEN_STATE_HAS_WING_FLY_RIGHT:	
		vx = KOOPAS_WALKING_SPEED;
		vy = -KOOPAS_FLYING_SPEED;
		break;
	case KOOPA_GREEN_STATE_HAS_WING_FLY_LEFT:
		vx = -KOOPAS_WALKING_SPEED;
		vy = -KOOPAS_FLYING_SPEED;
		break;
	case KOOPA_GREEN_STATE_HOLDING:
		last_state = KOOPA_GREEN_STATE_HOLDING;
		break;
	case KOOPA_GREEN_STATE_HOLDING_UP:
		last_state = KOOPA_GREEN_STATE_HOLDING_UP;
		break;
	}

}