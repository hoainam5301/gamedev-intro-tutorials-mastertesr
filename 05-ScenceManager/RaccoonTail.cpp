#include "RaccoonTail.h"
#include "ColorBox.h"


CRaccoonTail::CRaccoonTail()
{
	 //Mario = mario;
}

void CRaccoonTail::Render()
{
	RenderBoundingBox();
}

void CRaccoonTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y ;
	r =	x+8 ;
	b = y+6 ;
}
