#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	
public:

	int sceneid;
	float x;
	float y;
	bool isPortal;
	float l;
	float t;
	float r;
	float b;
	float state;

	int width;
	int height;

	CPortal(int sceneid, float x, float y, bool isPortal, float l, float t, float r, float b, float state);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetSceneId() { return sceneid; }
};