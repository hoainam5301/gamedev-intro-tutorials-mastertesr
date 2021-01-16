#pragma once


#include "Scence.h"
#include "Textures.h"
//#include "TileMap.h"
#include "Mario.h"
#include "Portal.h"
#include "Floor.h"
#include "ShakeTree.h"
#include "GameObject.h"
#include "StatusBar.h"
#include "SelectArrow.h"



class CSenceMap : public CScene
{

public:
	CMario* player;
	TileMap* wordmap;
	StatusBar* statusBar;
	int word_id;
	CPortal* current_portal;
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> mapPortals;
	CSenceMap(int id, LPCWSTR filePath);
	CMario* GetPlayer() { return player; }

	void Load();
	void Update(ULONGLONG dt);
	void Render();
	void Unload();

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TileMap(string line);

};

class CMapSceneKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CMapSceneKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};


