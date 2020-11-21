#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_RUNNING_ACC		0.0001f
#define MARIO_WALKING_ACC		0.0002f
#define MARIO_RUNNING_SPEED		0.2f
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

#define MARIO_BIG_STATE_IDLE_RIGHT			0
#define MARIO_BIG_STATE_WALKING_RIGHT			1
#define MARIO_BIG_STATE_RUNNING_RIGHT			2
#define MARIO_BIG_STATE_JUMP_RIGHT			3
#define MARIO_BIG_STATE_SITTING_RIGHT			4
#define MARIO_BIG_STATE_STOP_RIGHT			5
#define MARIO_BIG_STATE_FALLING_RIGHT			6
#define MARIO_BIG_STATE_KICK_RIGHT			7
#define MARIO_BIG_STATE_HOLDING_TURTLE_WALK_RIGHT	8
#define MARIO_BIG_STATE_HOLDING_TURTLE_IDLE_RIGHT	9
#define MARIO_BIG_STATE_HOLDING_TURTLE_JUMP_RIGHT	10
#define MARIO_BIG_STATE_HOLDING_TURTLE_RUNNING_RIGHT 11

#define MARIO_BIG_STATE_IDLE_LEFT				12
#define MARIO_BIG_STATE_WALKING_LEFT			13
#define MARIO_BIG_STATE_RUNNING_LEFT			14
#define MARIO_BIG_STATE_JUMP_LEFT				15
#define MARIO_BIG_STATE_SITTING_LEFT			16
#define MARIO_BIG_STATE_STOP_LEFT				17
#define MARIO_BIG_STATE_FALLING_LEFT			18
#define MARIO_BIG_STATE_KICK_LEFT				19
#define MARIO_BIG_STATE_HOLDING_TURTLE_WALK_LEFT	20
#define MARIO_BIG_STATE_HOLDING_TURTLE_IDLE_LEFT 21
#define MARIO_BIG_STATE_HOLDING_TURTLE_JUMP_LEFT 22
#define MARIO_BIG_STATE_HOLDING_TURTLE_RUNNING_LEFT 23
			
#define MARIO_SMALL_STATE_IDLE_RIGHT			24
#define MARIO_SMALL_STATE_WALKING_RIGHT		25
#define MARIO_SMALL_STATE_RUNNING_RIGHT		26
#define MARIO_SMALL_STATE_JUMP_RIGHT			27
#define MARIO_SMALL_STATE_STOP_RIGHT			28
#define MARIO_SMALL_STATE_KICK_RIGHT			29
#define MARIO_SMALL_STATE_HOLDING_TURTLE_WALK_RIGHT	30
#define MARIO_SMALL_STATE_HOLDING_TURTLE_IDLE_RIGHT	31
#define MARIO_SMALL_STATE_HOLDING_TURTLE_JUMP_RIGHT	32
#define MARIO_SMALL_STATE_HOLDING_TURTLE_RUNNING_RIGHT	33

#define MARIO_SMALL_STATE_IDLE_LEFT			34
#define MARIO_SMALL_STATE_WALKING_LEFT		35
#define MARIO_SMALL_STATE_RUNNING_LEFT		36
#define MARIO_SMALL_STATE_JUMP_LEFT			37
#define MARIO_SMALL_STATE_STOP_LEFT			38
#define MARIO_SMALL_STATE_KICK_LEFT			39
#define MARIO_SMALL_STATE_HOLDING_TURTLE_WALK_LEFT	40
#define MARIO_SMALL_STATE_HOLDING_TURTLE_IDLE_LEFT	41
#define MARIO_SMALL_STATE_HOLDING_TURTLE_JUMP_LEFT	42
#define MARIO_SMALL_STATE_HOLDING_TURTLE_RUNNING_LEFT	43

#define MARIO_RACCOON_STATE_IDLE_RIGHT			44
#define MARIO_RACCOON_STATE_WALK_RIGHT			45
#define MARIO_RACCOON_STATE_RUNNING_RIGHT		46
#define MARIO_RACCOON_STATE_JUMP_RIGHT			47
#define MARIO_RACCOON_STATE_STOP_RIGHT			48
#define MARIO_RACCOON_STATE_FIGHT_IDLE_RIGHT	49
#define MARIO_RACCOON_STATE_SITTING_RIGHT		50
#define MARIO_RACCOON_STATE_FALLING_RIGHT		51
#define MARIO_RACCOON_STATE_FALLING_ROCK_TAIL_RIGHT	52
#define MARIO_RACCOON_STATE_KEEP_FLYING_RIGHT		53
#define MARIO_RACCOON_STATE_FLYING_RIGHT		54
#define MARIO_RACCOON_STATE_KICK_RIGHT			55
#define MARIO_RACCOON_STATE_HOLDING_TURTLE_WALK_RIGHT	56
#define MARIO_RACCOON_STATE_HOLDING_TURTLE_IDLE_RIGHT	57
#define MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_RIGHT	58
#define MARIO_RACCOON_STATE_HOLDING_TURTLE_RUNNING_RIGHT	59

#define MARIO_RACCOON_STATE_IDLE_LEFT			60
#define MARIO_RACCOON_STATE_WALK_LEFT			61
#define MARIO_RACCOON_STATE_RUNNING_LEFT		62
#define MARIO_RACCOON_STATE_JUMP_LEFT			63
#define MARIO_RACCOON_STATE_STOP_LEFT			64
#define MARIO_RACCOON_STATE_FIGHT_IDLE_LEFT	65			
#define MARIO_RACCOON_STATE_SITTING_LEFT		66
#define MARIO_RACCOON_STATE_FALLING_LEFT		67
#define MARIO_RACCOON_STATE_FALLING_ROCK_TAIL_LEFT		68
#define MARIO_RACCOON_STATE_KEEP_FLYING_LEFT			69
#define MARIO_RACCOON_STATE_FLYING_LEFT		70
#define MARIO_RACCOON_STATE_KICK_LEFT			71
#define MARIO_RACCOON_STATE_HOLDING_TURTLE_WALK_LEFT	72
#define MARIO_RACCOON_STATE_HOLDING_TURTLE_IDLE_LEFT	73
#define MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_LEFT	74
#define MARIO_RACCOON_STATE_HOLDING_TURTLE_RUNNING_LEFT	75

#define MARIO_FIRE_STATE_IDLE_RIGHT			76
#define MARIO_FIRE_STATE_WALK_RIGHT			77
#define MARIO_FIRE_STATE_RUNNING_RIGHT		78
#define MARIO_FIRE_STATE_JUMP_RIGHT			79
#define MARIO_FIRE_STATE_STOP_RIGHT			80
#define MARIO_FIRE_STATE_FIGHT_RIGHT			81
#define MARIO_FIRE_STATE_FIGHT_WHILE_JUMPING_RIGHT	82
#define MARIO_FIRE_STATE_SITTING_RIGHT		83
#define MARIO_FIRE_STATE_FALLING_RIGHT		84
#define MARIO_FIRE_STATE_KICK_RIGHT			85
#define MARIO_FIRE_STATE_HOLDING_TURTLE_WALK_RIGHT	86
#define MARIO_FIRE_STATE_HOLDING_TURTLE_IDLE_RIGHT	87
#define MARIO_FIRE_STATE_HOLDING_TURTLE_JUMP_RIGHT	88
#define MARIO_FIRE_STATE_HOLDING_TURTLE_RUNNING_RIGHT	89

#define MARIO_FIRE_STATE_IDLE_LEFT			90
#define MARIO_FIRE_STATE_WALK_LEFT			91
#define MARIO_FIRE_STATE_RUNNING_LEFT         92
#define MARIO_FIRE_STATE_JUMP_LEFT			93
#define MARIO_FIRE_STATE_STOP_LEFT			94
#define MARIO_FIRE_STATE_FIGHT_LEFT			95
#define MARIO_FIRE_STATE_FIGHT_WHILE_JUMPING_LEFT	96
#define MARIO_FIRE_STATE_SITTING_LEFT			97
#define MARIO_FIRE_STATE_FALLING_LEFT			98
#define MARIO_FIRE_STATE_KICK_LEFT			99
#define MARIO_FIRE_STATE_HOLDING_TURTLE_WALK_LEFT	100
#define MARIO_FIRE_STATE_HOLDING_TURTLE_IDLE_LEFT	101
#define MARIO_FIRE_STATE_HOLDING_TURTLE_JUMP_LEFT	102
#define MARIO_FIRE_STATE_HOLDING_TURTLE_RUNNING_LEFT	103


//#define MARIO_RUNNING_RIGHT					4
//#define MARIO_RUNNING_LEFT					31

#define MARIO_ANI_DIE				104

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_RACCOON		3
#define MARIO_FIRE			4

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_WIDTH_RIGHT	20
#define MARIO_BIG_BBOX_HEIGHT 26
#define MARIO_BIG_BBOX_LEFT 5

#define MARIO_SMALL_BBOX_WIDTH  15
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_RACCOON_BBOX_WIDTH 15
#define MARIO_RACCOON_BBOX_HEIGHT 27
#define MARIO_RACCOON_BBOX_SIT	9
#define MARIO_RACCOON_BBOX_WIDTH_RIGHT	15
#define MARIO_RACCOON_BBOX_LEFT 7

#define MARIO_FIRE_BBOX_WIDTH 15
#define MARIO_FIRE_BBOX_HEIGHT 26
#define MARIO_FIRE_BBOX_WIDTH_KICK 21

#define MARIO_BIG_SIT_BBOX_HEIGHT	18
#define MARIO_RACCOON_SIT_BBOX_HEIGHT 18
#define MARIO_FIRE_SIT_BBOX_HEIGHT   18
#define MARIO_SIT_BBOX				8

#define MARIO_UNTOUCHABLE_TIME 3000
#define MARIO_TIME_FLY		   3500




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
	DWORD timestartfly;
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
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	//void startOnKey() {Start_on_Key = GetTickCount64(); }
	//bool Istimeout() { return GetTickCount64() - Start_on_Key >= 100; }
	void Reset();
	void SubRunningAcc();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};