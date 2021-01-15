#pragma once
#include "GameObject.h"

#define SHORT_PIPE	0
#define MEDIUM_PIPE	1
#define LONG_PIPE_GOTO_HIDDEN_MAP 4
#define PIPE_IN_HIDDEN_MAP	3
#define PIPE_WITH_LONG_ANI 2
#define PIPE_LONG_LONG 5

class CPipe : public CGameObject
{

public:
	int height;
	int width;
	int pipeType;
	CPipe(int width, int height,int pipetype);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

