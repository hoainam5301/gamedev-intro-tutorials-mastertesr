#include "Koopas.h"

CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
	{		
		bottom = top + KOOPAS_BBOX_HEIGHT_DIE;
	}
	else if (state == KOOPAS_ANI_DIE_AND_MOVE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else if (state==KOOPAS_STATE_WALKING)
		bottom = top + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;
	/*if (state == KOOPAS_STATE_WALKING)
	{*/
		if (vx < 0 && x < 512) {
			x = 512; vx = -vx;
		}

		if (vx > 0 && x > 592) {
			x = 592; vx = -vx;
		}
	/*}
	else if (state == KOOPAS_ANI_DIE_AND_MOVE)
	{

	}*/
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (vx > 0 && state != KOOPAS_ANI_DIE_AND_MOVE) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0 && state != KOOPAS_ANI_DIE_AND_MOVE) ani = KOOPAS_ANI_WALKING_LEFT;
	else if (state == KOOPAS_ANI_DIE_AND_MOVE) ani = KOOPAS_ANI_DIE_AND_MOVE;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		if(last_state==KOOPAS_ANI_DIE_AND_MOVE)
			y -= KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_ANI_DIE_AND_MOVE:
		last_state = KOOPAS_ANI_DIE_AND_MOVE;
		vx = 0.1*nx;
		break;
	}

}