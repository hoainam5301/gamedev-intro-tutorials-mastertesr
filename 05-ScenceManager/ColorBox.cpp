#include "ColorBox.h"


CColorBox::CColorBox(int width, int height) : CGameObject::CGameObject()
{
	this->width = width;
	this->height = height;
}

void CColorBox::Render()
{
	//RenderBoundingBox();
}

void CColorBox::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 16 * width;
	b = y + 16 * height;
}