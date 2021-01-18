#include "BoomerangBrother.h"
#include "Boomerang.h"
#include "MonneyEffect.h"


CBoomerangBrother::CBoomerangBrother(CMario* mario) : CGameObject()
{
	Mario = mario;
	GenerateRandomTurnToJump();
	SetState(BROTHER_STATE_MOVE_LEFT);
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(19));
}

void CBoomerangBrother::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	CGameObject::Update(dt, coObjects);
	vy += BROTHER_GRAVITY * dt;
	
	if (startX == 0)startX = x;
	if (startY == 0)startY = y;

	nx=GetPlayerDirection();
	if (nx > 0)
	{		
		if (x > startX + DISTANCE_BROTHER_GO )
		{
			if (!died)
			{
				countToJump++;
				SetState(BROTHER_STATE_IDLE_RIGHT);
				x = startX + DISTANCE_BROTHER_GO;
				
			}			
		}
		else if(x<startX)
		{
			if (!died)
			{
				countToJump++;
				SetState(BROTHER_STATE_IDLE_RIGHT);
				x = startX;
				
			}
		}
		if (GetTickCount64() - timeToRelax > 250 && timeToRelax != 0 && !died)
		{
			SetState(BROTHER_STATE_MOVE_RIGHT);
			timeToRelax = 0;
		}

		if (countToJump == randTurnToJump && isGrounded && !died)
		{
			SetState(BROTHER_STATE_JUMP_RIGHT);
			countToJump = 0;
		}
	}
	else
	{		
		
		if (x > startX + DISTANCE_BROTHER_GO )
		{
			if (!died)
			{
				countToJump++;
				x = startX + DISTANCE_BROTHER_GO;
				SetState(BROTHER_STATE_IDLE_LEFT);
			}
		}
		else if (x < startX)
		{
			if (!died)
			{
				countToJump++;
				x = startX;
				SetState(BROTHER_STATE_IDLE_LEFT);
			}
		}
		if (GetTickCount64() - timeToRelax > 250 && timeToRelax != 0 && !died)
		{
			SetState(BROTHER_STATE_MOVE_LEFT);
			timeToRelax = 0;
		}

		if (countToJump == randTurnToJump && !died && isGrounded)
		{

			SetState(BROTHER_STATE_JUMP_LEFT);
			countToJump = 0;
		}

	}
#pragma region Weapon Processing
	if (countBoomerang < 2)
	{
		if (countBoomerang == 0)
		{
			if (nx > 0)
			{
				SetState(BROTHER_STATE_ATTACK_RIGHT);
				CBoomerang* boomerang = new CBoomerang(1, Mario, this);
				boomerang->SetPosition(x + 10, y);
				listBoomerang.push_back(boomerang);
			}
			else
			{
				SetState(BROTHER_STATE_ATTACK_LEFT);
				CBoomerang* boomerang = new CBoomerang(-1, Mario, this);
				boomerang->SetPosition(x - 5, y);
				listBoomerang.push_back(boomerang);
			}			
			countBoomerang++;
			timereadyToThrow = GetTickCount64();
			//SetState(BROTHER_STATE_MOVING);
		}
		else if (countBoomerang == 1)
		{
			if (GetTickCount64() - timereadyToThrow > 700)
			{
				if (nx > 0)
				{
					SetState(BROTHER_STATE_ATTACK_RIGHT);
					CBoomerang* boomerang = new CBoomerang(1, Mario, this);
					boomerang->SetPosition(x + 10, y);
					listBoomerang.push_back(boomerang);
				}
				else
				{
					SetState(BROTHER_STATE_ATTACK_LEFT);
					CBoomerang* boomerang = new CBoomerang(-1, Mario, this);
					boomerang->SetPosition(x - 5, y);
					listBoomerang.push_back(boomerang);
				}
				countBoomerang++;
			}
			else
			{
				if (nx > 0)
					SetState(BROTHER_STATE_MOVE_RIGHT);
				else
					SetState(BROTHER_STATE_MOVE_LEFT);
			}
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

	for (UINT i = 0; i < listBoomerang.size(); i++)
		listBoomerang[i]->Update(dt, coObjects);
#pragma endregion

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		isGrounded = false;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!
		if (!died)
		{
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.1f;
		}
		if (died)
		{
			y += dy;
		}
		if (ny != 0 && !died)
		{
			vy = 0;
			isGrounded = true;
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBoomerangBrother::Render()
{
	if (!died)
		animation_set->at(state)->Render(x, y);
	else
	{
		if(GetTickCount64()-timeRenderDie<500)
			animation_set->at(state)->Render(x, y);
		else
			isdone = true;
	}

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

void CBoomerangBrother::GetBoundingBox(float& l, float& t, float& r, float& b)
{	
	l = x;
	t = y;
	r = l + BROTHER_BBOX_WIDTH;
	b = t + BROTHER_BBOX_HEIGHT;
}

void CBoomerangBrother::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BROTHER_STATE_JUMP_RIGHT:
		vy = -BROTHER_JUMP_SPEED;
		GenerateRandomTurnToJump();
		break;
	case BROTHER_STATE_JUMP_LEFT:
		vy = -BROTHER_JUMP_SPEED;
		GenerateRandomTurnToJump();
		break;
	case BROTHER_STATE_MOVE_RIGHT:
		if (x == startX)
			vx = BROTHER_SPEED_GO_AROUND;
		else if (x == startX + DISTANCE_BROTHER_GO)
			vx = -BROTHER_SPEED_GO_AROUND;
		if (isHoldingBoomerang)
			state = BROTHER_STATE_ATTACK_RIGHT;
		break;
	case BROTHER_STATE_MOVE_LEFT:
		if (x == startX)
			vx = BROTHER_SPEED_GO_AROUND;
		else if (x == startX + DISTANCE_BROTHER_GO)
			vx = -BROTHER_SPEED_GO_AROUND;
		if (isHoldingBoomerang)
			state = BROTHER_STATE_ATTACK_LEFT;
		break;
	case BROTHER_STATE_DIE_RIGHT:	
		timeRenderDie = GetTickCount64();
		break;
	case BROTHER_STATE_DIE_LEFT:
		timeRenderDie = GetTickCount64();
		break;
	case BROTHER_STATE_IDLE_RIGHT:
		timeToRelax = GetTickCount64();
		vx = 0;
		break;
	case BROTHER_STATE_IDLE_LEFT:
		timeToRelax = GetTickCount64();
		vx = 0;
		break;
	case BROTHER_STATE_ATTACK_RIGHT:
		break;
	case BROTHER_STATE_ATTACK_LEFT:
		break;
	}
}

void CBoomerangBrother::CreateBoomerang()
{
	
	CBoomerang* boomerang = new CBoomerang(1,Mario,this);
	
	/*if (nx > 0)
	{*/
		boomerang->SetPosition(x + 5, y);
		boomerang->nx = 1;
		boomerang->SetState(BOOMERANG_STATE_MOVE_RIGHT);
	/*}
	else
	{
		boomerang->SetPosition(x - 10, y);
		boomerang->nx = -1;
		boomerang->SetState(BOOMERANG_STATE_MOVE_LEFT);
	}*/
	listBoomerang.push_back(boomerang);
}

void CBoomerangBrother::GenerateRandomTurnToJump()
{
	randTurnToJump = rand() % (6 - 5 + 1) + 5;
}

int CBoomerangBrother::GetPlayerDirection()
{
	if (Mario->leftOfMario >= this->x)
		return 1;
	else
		return -1;
}

CBoomerangBrother::~CBoomerangBrother()
{
}
