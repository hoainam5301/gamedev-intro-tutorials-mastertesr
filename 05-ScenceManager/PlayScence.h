#pragma once

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
//#include "TileMap.h"
#include "RaccoonTail.h"
#include "BrokenBrick.h"
#include "Coin.h"
#include "Portal.h"
#include "Floor.h"
#include "ColorBox.h"
#include "GiantPiranhaPlantBite.h"
#include "MonneyEffect.h"
#include "StatusBar.h"
#include "Pipe.h"
#include "Grid.h"
#include "MovingWood.h"
#include "BoomerangBrother.h"


class CPlayScene: public CScene
{
protected: 
	CMario *player;	
	CItems	*items;
	//CWeapon* weapon;
	TileMap* map;
	CRaccoonTail* tail=new CRaccoonTail();
	CCoin* coin =NULL;
	StatusBar* statusBar;

	CGrid* gridObjMove;
	CGrid* gridObjIdle;
	vector<LPGAMEOBJECT> listAllObjMove;
	vector<LPGAMEOBJECT> listAllObjIdle;
	vector<LPGAMEOBJECT> listObjIdle;	//list obj dung yen
	vector<LPGAMEOBJECT> listObjMove;
	vector<LPGAMEOBJECT> listCoin;    //Coin xoay o dam may
	vector<LPGAMEOBJECT> leafTree;		//tach biet leaftree voi ca items khac de khoi bi loi ve sau gach

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> listitems;
	vector<LPGAMEOBJECT> listweapon;	//dan
	vector<LPGAMEOBJECT> listcoin;  //coin do tranformation 

	ULONGLONG timeTranformation;
	//vector<LPGAMEOBJECT> listbrokenbrick;

	CItems* MadeItems(float x, float y)
	{
		CItems* a = new CItems(y,x);
		a->SetPosition(x, y);
		a->id_items = RandomItems();
		a->SetState(a->id_items);
		return a;	
	}
	CWeapon* MadeWeapon(float x, float y,int nx)
	{
		CWeapon* a = new CWeapon(x, y,nx);
		//a->nx = player->nx;
		a->SetPosition(x, y);
		return a;
	}
	
	string listTextEndGame = "YOU GOT A CARD";
	string lisstTextClear = "COURSE CLEAR ";

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TileMap(string line);

	int RandomItems();

	
public: 
	CPlayScene(int id, LPCWSTR filePath);
	LPSPRITE text;	
	virtual void Load();
	virtual void Update(ULONGLONG dt);
	virtual void Render();
	virtual void Unload();
	void SetCamSpeedY(ULONGLONG dt);
	void InsertObjToGrid();
	void MakeItemEndGame();
	void RenderTextEndGame();
	//int GetLeverPlayer();
	bool hasMakeRandomItemsEndGame;
	float cx, cy;
	float camY;
	float camX;
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

