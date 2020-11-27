#include "GameObject.h"
#include "PieceBrick.h"

#define STATE_DESTROYED	0
#define STATE_BRICK_NORMAL 1
#define STATE_COIN_ROTATE  2
#define STATE_COIN_NO_ROTATE 3


class CBrokenBrick : public CGameObject
{
public:
	vector<LPGAMEOBJECT> listPiece;
	ULONGLONG timeTranformation;
	bool vanish;
	bool hasTranformation;
	bool isDestroyed;
	bool tranformation;
	/*CBrokenBrickPiece* topLeftPiece;
	CBrokenBrickPiece* topRightPiece;
	CBrokenBrickPiece* bottomLeftPiece;
	CBrokenBrickPiece* bottomRightPiece;*/

	CBrokenBrick(int id_state);
	virtual void Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
};