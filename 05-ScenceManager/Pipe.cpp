#include "Pipe.h"
CPipe::CPipe(int width, int height,int pipetype) : CGameObject::CGameObject()
{
	this->width = width;
	this->height = height;
	this->pipeType = pipetype;
}

void CPipe::Render()
{
	int ani;
	if (pipeType == SHORT_PIPE)
		ani = SHORT_PIPE;
	else if (pipeType == MEDIUM_PIPE)
		ani = MEDIUM_PIPE;
	else if (pipeType == LONG_PIPE_GOTO_HIDDEN_MAP)
		ani = LONG_PIPE_GOTO_HIDDEN_MAP;
	else if (pipeType == PIPE_IN_HIDDEN_MAP)
		ani = PIPE_IN_HIDDEN_MAP;
	else if (pipeType == PIPE_LONG_LONG)
		ani = PIPE_LONG_LONG;
	else
		ani = PIPE_WITH_LONG_ANI;
	

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16 * width;
	b = y + 16 * height;
}