#pragma once
#include "GameObject.h"
#include "Utils.h"

#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_RUNNING_ACC		0.00015f
#define MARIO_WALKING_ACC		0.0002f
#define MARIO_RUNNING_SPEED		0.25f
#define MARIO_SUB_WALKING_ACC	0.0005f
#define MARIO_SUB_RUNNING_ACC	0.012f
#define MARIO_MIN_SPEED_TO_STOP 0.06f

#define MARIO_JUMP_SPEED_Y		0.275f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.0006f
#define MARIO_DIE_DEFLECT_SPEED	 0.2f
#define MARIO_FLY_SPEED_Y		0.2f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	1
#define MARIO_STATE_WALKING_LEFT	2
#define MARIO_STATE_RUN				300

#define MARIO_STATE_DIE				400

#define MARIO_ANI_BIG_IDLE_RIGHT			0
#define MARIO_ANI_BIG_WALKING_RIGHT			1
#define MARIO_ANI_BIG_RUNNING_RIGHT			2
#define MARIO_ANI_BIG_JUMP_RIGHT			3
#define MARIO_ANI_BIG_SITTING_RIGHT			4
#define MARIO_ANI_BIG_STOP_RIGHT			5
#define MARIO_ANI_BIG_FALLING_RIGHT			6
#define MARIO_ANI_BIG_KICK_RIGHT			7

#define MARIO_ANI_BIG_IDLE_LEFT				8
#define MARIO_ANI_BIG_WALKING_LEFT			9
#define MARIO_ANI_BIG_RUNNING_LEFT			10
#define MARIO_ANI_BIG_JUMP_LEFT				11
#define MARIO_ANI_BIG_SITTING_LEFT			12
#define MARIO_ANI_BIG_STOP_LEFT				13
#define MARIO_ANI_BIG_FALLING_LEFT			14
#define MARIO_ANI_BIG_KICK_LEFT				15
			
#define MARIO_ANI_SMALL_IDLE_RIGHT			16
#define MARIO_ANI_SMALL_WALKING_RIGHT		17
#define MARIO_ANI_SMALL_RUNNING_RIGHT		18
#define MARIO_ANI_SMALL_JUMP_RIGHT			19
#define MARIO_ANI_SMALL_STOP_RIGHT			20
#define MARIO_ANI_SMALL_KICK_RIGHT			21

#define MARIO_ANI_SMALL_IDLE_LEFT			22
#define MARIO_ANI_SMALL_WALKING_LEFT		23
#define MARIO_ANI_SMALL_RUNNING_LEFT		24
#define MARIO_ANI_SMALL_JUMP_LEFT			25
#define MARIO_ANI_SMALL_STOP_LEFT			26
#define MARIO_ANI_SMALL_KICK_LEFT			27

#define MARIO_RACCON_ANI_IDLE_RIGHT			28
#define MARIO_RACCON_ANI_WALK_RIGHT			29
#define MARIO_RACCON_ANI_RUNNING_RIGHT		30
#define MARIO_RACCON_ANI_JUMP_RIGHT			31
#define MARIO_RACCON_ANI_STOP_RIGHT			32
#define MARIO_RACCON_ANI_FIGHT_IDLE_RIGHT	33
#define MARIO_RACCON_ANI_SITTING_RIGHT		34
#define MARIO_RACCON_ANI_FALLING_RIGHT		35
#define MARIO_RACCON_ANI_FALLING_ROCK_TAIL_RIGHT	36
#define MARIO_RACCON_ANI_KEEP_FLYING_RIGHT		37
#define MARIO_RACCON_ANI_FLYING_RIGHT		38
#define MARIO_RACCON_ANI_KICK_RIGHT			39

#define MARIO_RACCON_ANI_IDLE_LEFT			40
#define MARIO_RACCON_ANI_WALK_LEFT			41
#define MARIO_RACCON_ANI_RUNNING_LEFT		42
#define MARIO_RACCON_ANI_JUMP_LEFT			43
#define MARIO_RACCON_ANI_STOP_LEFT			44
#define MARIO_RACCON_ANI_FIGHT_IDLE_LEFT	45			
#define MARIO_RACCON_ANI_SITTING_LEFT		46
#define MARIO_RACCON_ANI_FALLING_LEFT		47
#define MARIO_RACCON_ANI_FALLING_ROCK_TAIL_LEFT		48
#define MARIO_RACCON_ANI_KEEP_FLYING_LEFT			49
#define MARIO_RACCON_ANI_FLYING_LEFT		50
#define MARIO_RACCON_ANI_KICK_LEFT			51

#define MARIO_FIRE_ANI_IDLE_RIGHT			52
#define MARIO_FIRE_ANI_WALK_RIGHT			53
#define MARIO_FIRE_ANI_RUNNING_RIGHT		54
#define MARIO_FIRE_ANI_JUMP_RIGHT			55
#define MARIO_FIRE_ANI_STOP_RIGHT			56
#define MARIO_FIRE_ANI_FIGHT_RIGHT			57
#define MARIO_FIRE_ANI_FIGHT_WHILE_JUMPING_RIGHT	58
#define MARIO_FIRE_ANI_SITTING_RIGHT		59
#define MARIO_FIRE_ANI_FALLING_RIGHT		60
#define MARIO_FIRE_ANI_KICK_RIGHT			61

#define MARIO_FIRE_ANI_IDLE_LEFT			62
#define MARIO_FIRE_ANI_WALK_LEFT			63
#define MARIO_FIRE_ANI_RUNNING_LEFT         64
#define MARIO_FIRE_ANI_JUMP_LEFT			65
#define MARIO_FIRE_ANI_STOP_LEFT			66
#define MARIO_FIRE_ANI_FIGHT_LEFT			67
#define MARIO_FIRE_ANI_FIGHT_WHILE_JUMPING_LEFT	68
#define MARIO_FIRE_ANI_SITTING_LEFT			69
#define MARIO_FIRE_ANI_FALLING_LEFT			70
#define MARIO_FIRE_ANI_KICK_LEFT			71

//#define MARIO_RUNNING_RIGHT					4
//#define MARIO_RUNNING_LEFT					31

#define MARIO_ANI_DIE				72

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_RACCON		3
#define MARIO_FIRE			4

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_WIDTH_RIGHT	20
#define MARIO_BIG_BBOX_HEIGHT 27
#define MARIO_BIG_BBOX_LEFT 5

#define MARIO_SMALL_BBOX_WIDTH  15
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_RACCON_BBOX_WIDTH 15
#define MARIO_RACCON_BBOX_HEIGHT 28
#define MARIO_RACCON_BBOX_SIT	9
#define MARIO_RACCON_BBOX_WIDTH_RIGHT	15
#define MARIO_RACCON_BBOX_LEFT 7

#define MARIO_FIRE_BBOX_WIDTH 15
#define MARIO_FIRE_BBOX_HEIGHT 27
#define MARIO_FIRE_BBOX_WIDTH_KICK 21

#define MARIO_BIG_SIT_BBOX_HEIGHT	18
#define MARIO_RACCON_SIT_BBOX_HEIGHT 19
#define MARIO_FIRE_SIT_BBOX_HEIGHT   18
#define MARIO_SIT_BBOX				9

#define MARIO_UNTOUCHABLE_TIME 5000




class CMario : public CGameObject
{
	
	
	int untouchable;
	DWORD untouchable_start;
	

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	//DWORD Start_on_Key;
	bool is_Grounded;
	bool isMaxSpeed;
	bool Firstspaceup;
	//bool gravity_raccon;
	int level;
	bool isJumping, isSitting;// isFalling;
	bool isFlying,isRunning;
	bool use_Weapon;
	int lastnx;
	float last_vx;
	bool isWaitingForAni;
	bool isFalling;
	CMario(float x = 0.0f, float y = 0.0f);
	void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	void Render();

	virtual void Collision_items(vector<LPGAMEOBJECT>* listitem);

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	//void startOnKey() {Start_on_Key = GetTickCount(); }
	//bool Istimeout() { return GetTickCount() - Start_on_Key >= 100; }
	void Reset();
	void SubRunningAcc();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};