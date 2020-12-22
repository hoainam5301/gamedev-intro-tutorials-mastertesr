#include "ShakeTree.h"

CShakeTree::CShakeTree()
{	
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(2));
}

void CShakeTree::Update(ULONGLONG dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
}

void CShakeTree::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	
}

void CShakeTree::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

CShakeTree::~CShakeTree()
{
}

