#include "GameObject.h"


class CColorBox : public CGameObject
{
	int height;
	int width;
public:
	CColorBox(int width, int height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

};