#include "GameObject.h"
#include "PieceBrick.h"

#define STATE_DESTROYED	1


class CBrokenBrick : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listPiece;
	bool vanish;
	bool isDestroyed;

	/*CBrokenBrickPiece* topLeftPiece;
	CBrokenBrickPiece* topRightPiece;
	CBrokenBrickPiece* bottomLeftPiece;
	CBrokenBrickPiece* bottomRightPiece;*/

	CBrokenBrick();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};