#include "Portal.h"


CPortal::CPortal(int sceneid, float x, float y, bool isPortal, float l, float t, float r, float b,float state )
{
	this->sceneid = sceneid;
	this->x = x;
	this->y = y;
	this->state = state;
	this->isPortal = isPortal;
	this->l = l;
	this->t = t;
	this->r = r;
	this->b = b;

	this->width = 16;
	this->height = 16;
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}