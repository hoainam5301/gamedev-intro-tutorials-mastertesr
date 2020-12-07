#include "Pipe.h"
CPipe::CPipe(int width, int height,int pipetype) : CGameObject::CGameObject()
{
	this->width = width;
	this->height = height;
	this->pypeType = pipetype;
}

void CPipe::Render()
{
	RenderBoundingBox();
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16 * width;
	b = y + 16 * height;
}