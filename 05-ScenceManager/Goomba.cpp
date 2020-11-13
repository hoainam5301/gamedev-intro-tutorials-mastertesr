#include "Goomba.h"
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state != GOOMBA_STATE_DIE /*|| state!=GOOMBA_STATE_DIE_FLY*/)
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
	/*if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}*/	
	CGameObject::Update(dt);
	vy += 0.00005 * dt;
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
		//chamsan = true;
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block every object first!


		y += min_ty * dy + ny * 0.5f;
		x += min_tx * dx + nx * 0.5f;
		/*if (ny < 0)
		{
			vy = -MOVING_SPEED;
		}*/

		if (ny != 0)
			vy = 0;
		//if (coEvents.size() <= 3)
		//{
		//	x += dx;
		//}
		//else
		//	//doihuong *= -1;
		//	if (nx != 0) this->isdone = true;

		/*if (ny==0 && nx!=0)
		{
			doihuong *= -1;
			x += nx * 2;
		}*/



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
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}
	else if (state == GOOMBA_STATE_DIE_FLY)
		ani = GOOMBA_ANI_DIE_FLY;
	if(state==GOOMBA_STATE_WALKING)
		animation_set->at(ani)->Render(x,y);
	else if (state == GOOMBA_STATE_DIE)
	{
		if(timerenderanidie==0)
			timerenderanidie = GetTickCount64();
		if (GetTickCount64() - timerenderanidie < 200)
			animation_set->at(ani)->Render(x, y);
	}
	else if (state == GOOMBA_STATE_DIE_FLY)
	{
		if (timerenderanidie == 0)
			timerenderanidie = GetTickCount64();
		if (GetTickCount64() - timerenderanidie < 200)
			animation_set->at(ani)->Render(x, y);
	}

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
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_DIE_FLY:
			vx = -GOOMBA_WALKING_SPEED + 0.04f;
			vy = -0.15;
			break;
	}
}
