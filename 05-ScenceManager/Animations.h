#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Sprites.h"

/*
Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	ULONGLONG time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	ULONGLONG GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame *LPANIMATION_FRAME;

class CAnimation
{
	ULONGLONG aniStartTime;
	ULONGLONG lastFrameTime;
	int currentFrame;
	int defaultTime;
	ULONGLONG totalFrameTime=0;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) {
	this->aniStartTime = NULL; 
	this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, ULONGLONG time = 0);
	void Reset() { currentFrame = -1; }
	void Render(float x, float y, int alpha = 255);
	void SetAniStartTime(ULONGLONG t) { aniStartTime = t; }
	bool IsOver() { return GetTickCount64() - aniStartTime >= totalFrameTime; }
	bool IsReadyToFight() { return GetTickCount64() - aniStartTime >= totalFrameTime - 150; }
};

typedef CAnimation *LPANIMATION;

class CAnimations
{
	static CAnimations * __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();

	static CAnimations * GetInstance();
};

typedef vector<LPANIMATION> CAnimationSet;

typedef CAnimationSet* LPANIMATION_SET;

/*
	Manage animation set database
*/
class CAnimationSets
{
	static CAnimationSets * __instance;

	unordered_map<int, LPANIMATION_SET> animation_sets;

public:
	CAnimationSets();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);


	static CAnimationSets * GetInstance();
};