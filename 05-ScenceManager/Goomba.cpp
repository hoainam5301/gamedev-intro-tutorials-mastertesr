#include "Goomba.h"
#include "ColorBox.h"

CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state != GOOMBA_STATE_DIE && state!=GOOMBA_STATE_DIE_FLY)
	{
		left = x;
		top = y;
		right = x + GOOMBA_BBOX_WIDTH;
		bottom = y + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 	
	
	CGameObject::Update(dt);
	if(GetState()!=GOOMBA_STATE_DIE)
		vy += 0.0005 * dt;
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


		y += min_ty * dy + ny * 0.5f;
		x += min_tx * dx + nx * 0.5f;
	

		if (ny != 0)
			vy = 0;
	



		//Collision logic with other objects

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CColorBox*>(e->obj))
			{
				if (e->nx != 0)
					x += dx;
			}
			else if (e->nx != 0)
				vx = -vx;
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGoomba::Render()
{	
	if (state == GOOMBA_STATE_DIE)
	{
		if(timerenderanidie==0)
			timerenderanidie = GetTickCount();
		if (GetTickCount() - timerenderanidie < 300)
			animation_set->at(state)->Render(x, y);
	}
	else 
		animation_set->at(state)->Render(x, y);
	//RenderBoundingBox();
}
CGoomba::CGoomba(float width, float height)
{
	this->x = width;
	this->y = height;
}
void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_DIE_FLY:
			vx = -vx;
			vy = -0.15;
			break;
	}
}
