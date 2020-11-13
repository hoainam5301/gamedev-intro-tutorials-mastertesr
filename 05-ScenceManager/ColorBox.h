#include "GameObject.h"


class CColorBox : public CGameObject
{

public:
	int height;
	int width;
	CColorBox(int width, int height);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

};