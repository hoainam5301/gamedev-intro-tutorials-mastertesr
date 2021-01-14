#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"
#include "Utils.h"
#include "Game.h"
#include "Textures.h"
#include "TileMap.h"



using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:

	//int ani;

	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;
	
	//bool isDestroyed;
	int nx;	
	int ny;
	bool isExplode;
	bool isHolding;
	bool checkOnCam;

	int id_broken_state; //set trang thai cua broken brick luc moi khoi tao
	int id_items;		
	int id_koopa;		//dung de phan biet koopa red va koopa green
	int id_goomba;		//dung de phan biet goomba do vs goomba binh thuong
	int id_brick_items;	//id cua gach co items(idgachranam)
	int id_giantpiranha;// id cua giant piranha, dung de phan biet mau xanh vs do ban dan

	bool isdone;
	bool loadFireball;
	bool makeEffect;
	bool makeLife;

	int state;

	ULONGLONG dt; 
	
	LPANIMATION_SET animation_set;

public: 

	bool CheckAABB(float l_a, float t_a, float r_a, float b_a, float l_b, float t_b, float r_b, float b_b)
	{
		return (l_a < r_b&& r_a > l_b && t_a < b_b&& b_a > t_b);
	}
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	void ResetAni();

	~CGameObject();
};

