#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "GiantPiranhaPlant.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Items.h"
#include "Weapon.h"
#include "TileMap.h"

class CPlayScene: public CScene
{
protected: 
	CMario *player;	
	CItems	*items;
	CWeapon* weapon;
	TileMap* map;
	// A play scene has to have player, right? 
	// *gachthuong;
	CBrick* gachthuong;
	vector<LPGAMEOBJECT> objects;

	vector<LPGAMEOBJECT> listitems;
	vector<LPGAMEOBJECT> listweapon;

	CItems* taonam(float x, float y)
	{
		CItems* a = new CItems(y,x);
		a->SetPosition(x, y);
		a->id_items = RandomItems();
		a->SetState(a->id_items);
		return a;	
	}
	CWeapon* tao_weapon(float x, float y,CMario* mario)
	{
		CWeapon* a = new CWeapon(x, y,player);
		a->SetPosition(x, y);
		return a;
	}
	

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

	int RandomItems();

	
public: 
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario * GetPlayer() { return player; } 
	CItems* GetItems()	 { return items; }

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) ;
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};
