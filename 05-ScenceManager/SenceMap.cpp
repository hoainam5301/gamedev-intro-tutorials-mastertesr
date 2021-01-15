#include "SenceMap.h"
#include "Game.h"
#include <iostream>
#include <fstream>

using namespace std;

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TILEMAP 7


#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_SHAKE_TREE 1
#define OBJECT_TYPE_FLOOR 2
#define OBJECT_TYPE_PORTAL 3

#define MAX_SCENE_LINE 1024



CSenceMap::CSenceMap(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new  CMapSceneKeyHandler(this);
}

//void CSenceMap::_ParseSection_TileMap(string line)
//{
//
//	vector<string> tokens = split(line);
//	if (tokens.size() < 9) return;
//	int ID = atoi(tokens[0].c_str());
//	wstring file_texture = ToWSTR(tokens[1]);
//	wstring file_path = ToWSTR(tokens[2]);
//	int row_on_textures = atoi(tokens[3].c_str());
//	int col_on_textures = atoi(tokens[4].c_str());
//	int row_on_tile_map = atoi(tokens[5].c_str());
//	int col_on_tile_map = atoi(tokens[6].c_str());
//	int tile_width = atoi(tokens[7].c_str());
//	int tile_height = atoi(tokens[8].c_str());
//	//int texID = atoi(tokens[0].c_str());
//	map = new TileMap(ID, file_texture.c_str(), file_path.c_str(), row_on_textures, col_on_textures, row_on_tile_map, col_on_tile_map, tile_width, tile_height);
//
//}

void CSenceMap::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CSenceMap::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CSenceMap::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CSenceMap::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CSenceMap::_ParseSection_TileMap(string line)
{

	vector<string> tokens = split(line);
	if (tokens.size() < 9) return;
	int ID = atoi(tokens[0].c_str());
	wstring file_texture = ToWSTR(tokens[1]);
	wstring file_path = ToWSTR(tokens[2]);
	int row_on_textures = atoi(tokens[3].c_str());
	int col_on_textures = atoi(tokens[4].c_str());
	int row_on_tile_map = atoi(tokens[5].c_str());
	int col_on_tile_map = atoi(tokens[6].c_str());
	int tile_width = atoi(tokens[7].c_str());
	int tile_height = atoi(tokens[8].c_str());

	//int texID = atoi(tokens[0].c_str());
	wordmap = new TileMap(ID, file_texture.c_str(), file_path.c_str(), row_on_textures, col_on_textures, row_on_tile_map, col_on_tile_map, tile_width, tile_height);

}

void CSenceMap::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	CGameObject* obj = NULL;

	switch (object_type)
	{
		case OBJECT_TYPE_MARIO:
		/*if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}*/
		player = CMario::GetInstance();
		statusBar = new StatusBar(player);
		player->SetPosition(x, y);
		player->SetAnimationSet(ani_set);
		DebugOut(L"[INFO] Player object created!\n");
		break;
		case OBJECT_TYPE_SHAKE_TREE:
			obj = new CShakeTree();
			break;
		case OBJECT_TYPE_FLOOR:
		{
			float width = atof(tokens[4].c_str());
			float height = atof(tokens[5].c_str());
			obj = new CFloor(width, height);
			break;
		}
		case OBJECT_TYPE_PORTAL:
		{
			int id = atof(tokens[1].c_str());
			float x = atof(tokens[2].c_str());
			float y = atof(tokens[3].c_str());

			int isPortal = atof(tokens[4].c_str());
			int l = atof(tokens[5].c_str());

			int t = atoi(tokens[6].c_str());
			int r = atoi(tokens[7].c_str());
			int b = atoi(tokens[8].c_str());
			int state = atoi(tokens[9].c_str());
			obj = new CPortal(id, x, y, isPortal, l, t, r, b, state);
			mapPortals.push_back(obj);
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	if (obj)
	{
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
	}	
}

void CSenceMap::Load()
{
	isLoad = true;
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[TILEMAP]")
		{
			section = SCENE_SECTION_TILEMAP;
			continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TILEMAP: _ParseSection_TileMap(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CSenceMap::Update(ULONGLONG dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	if (player->isAutoGoRight)
	{
		for (int i = 0; i < mapPortals.size(); i++)
		{
			LPGAMEOBJECT p = mapPortals[i];
			CPortal* portal = dynamic_cast<CPortal*>(p);
			if (portal->sceneid == player->sceneIdPresent)
			{
				if (portal->r != -1)
				{
					player->sceneIdGoToRight = portal->r;
					for (int j = 0; j < mapPortals.size(); j++)
					{
						LPGAMEOBJECT p = mapPortals[j];
						CPortal* Portal = dynamic_cast<CPortal*>(p);
						if (Portal->sceneid == portal->r)
						{
							if (Portal->isPortal)
								player->isStandOnPortal = true;
							player->posXOfNextPortalGoRight = Portal->x;
							/*player->posXOfPortal = Portal->x;
							player->posYOfPortal = Portal->y;*/
						}
					}
					player->vx = 0.1;	
					player->isGoTo = true;
				}
			}
		}
	}
	else if (player->isAutoGoTop)
	{
		for (int i = 0; i < mapPortals.size(); i++)
		{
			LPGAMEOBJECT p = mapPortals[i];
			CPortal* portal = dynamic_cast<CPortal*>(p);
			if (portal->sceneid == player->sceneIdPresent)
			{
				if (portal->t != -1)
				{
					player->sceneIdGoToUp = portal->t;
					for (int j = 0; j < mapPortals.size(); j++)
					{
						LPGAMEOBJECT p = mapPortals[j];
						CPortal* Portal = dynamic_cast<CPortal*>(p);
						if (Portal->sceneid == portal->t)
						{
							if (Portal->isPortal)
								player->isStandOnPortal = true;
							player->posYOfNextPortalGoUp = Portal->y;
							player->posXOfPortal = Portal->x;
							//player->posYOfPortal = Portal->y;
						}
					}
					player->vy = -0.1;
					player->isGoTo = true;
				}
			}
		}
	}
	else if (player->isAutoGoLeft)
	{
		for (int i = 0; i < mapPortals.size(); i++)
		{
			LPGAMEOBJECT p = mapPortals[i];
			CPortal* portal = dynamic_cast<CPortal*>(p);
			if (portal->sceneid == player->sceneIdPresent)
			{
				if (portal->l != -1)
				{
					player->sceneIdGoToLeft = portal->l;
					for (int j = 0; j < mapPortals.size(); j++)
					{
						LPGAMEOBJECT p = mapPortals[j];
						CPortal* Portal = dynamic_cast<CPortal*>(p);
						if (Portal->sceneid == portal->l)
						{
							if (Portal->isPortal)
								player->isStandOnPortal = true;
							player->posXOfNextPortalGoLeft = Portal->x;
							/*player->posXOfPortal = Portal->x;
							player->posYOfPortal = Portal->y;*/
						}
					}
					player->vx = -0.1;
					player->isGoTo = true;
				}
			}
		}
	}
	else if (player->isAutoGoDown)
	{
		for (int i = 0; i < mapPortals.size(); i++)
		{
			LPGAMEOBJECT p = mapPortals[i];
			CPortal* portal = dynamic_cast<CPortal*>(p);
			if (portal->sceneid == player->sceneIdPresent)
			{
				if (portal->b != -1)
				{
					player->sceneIdGoToDown = portal->b;
					for (int j = 0; j < mapPortals.size(); j++)
					{
						LPGAMEOBJECT p = mapPortals[j];
						CPortal* Portal = dynamic_cast<CPortal*>(p);
						if (Portal->sceneid == portal->b)
						{
							if (Portal->isPortal)
								player->isStandOnPortal = true;
							player->posYOfNextPortalGoDown = Portal->y;
							player->posXOfPortal = Portal->x;
						//	player->posYOfPortal= Portal->y;
						}
					}
					player->vy = 0.1;
					player->isGoTo = true;
				}
			}
		}
	}
	for (int j = 0; j < mapPortals.size(); j++)
	{
		LPGAMEOBJECT p = mapPortals[j];
		CPortal* Portal = dynamic_cast<CPortal*>(p);
		if (Portal->sceneid==player->sceneIdPresent)
		{
			player->posXOfPortal = Portal->x;
			player->posYOfPortal = Portal->y;
		}
	}
	player->SetState(MARIO_STATE_IN_WORD_MAP);
	player->UpdateInScenceMap(dt);
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}
	
	CGame::GetInstance()->cam_y = 0;
	CGame::GetInstance()->cam_x = 0;
	statusBar->Update(dt, CGame::GetInstance()->cam_x, CGame::GetInstance()->cam_y-40);
}

void CSenceMap::Render()
{
	wordmap->Draw();
	for (int i = objects.size() - 1; i >= 0; i--)
		objects[i]->Render();
	player->Render();
	statusBar->Render();
}

void CSenceMap::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	/*for (int i = 0; i < mapPortals.size(); i++)
		delete mapPortals[i];*/
	mapPortals.clear();
	objects.clear();
//	player = NULL;
}

void CMapSceneKeyHandler::OnKeyUp(int keycode)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	//CMario* mario = ((CSenceMap*)scence)->GetPlayer();

}

void CMapSceneKeyHandler::OnKeyDown(int keycode)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	if (mario->isGoTo)
		return;
	switch (keycode) {
	case DIK_C:
		mario->SetState(MARIO_STATE_IDLE);
		if (mario->isStandOnPortal && mario->vx == 0)
		{
			if (mario->sceneIdPresent == 4)
				mario->inMapTwo = true;
			CGame::GetInstance()->SwitchScene(mario->sceneIdPresent);
		}
		break;
	case DIK_LEFT:
		mario->isAutoGoLeft = true;
		mario->isAutoGoDown = false;
		mario->isAutoGoTop = false;
		mario->isAutoGoRight = false;
		break;
	case DIK_RIGHT:
		mario->isAutoGoRight = true;
		mario->isAutoGoLeft = false;
		mario->isAutoGoDown = false;
		mario->isAutoGoTop = false;
		break;
	case DIK_UP:
		mario->isAutoGoTop = true;
		mario->isAutoGoLeft = false;
		mario->isAutoGoDown = false;
		mario->isAutoGoRight = false;
		break;
	case DIK_DOWN:
		mario->isAutoGoDown=true;
		mario->isAutoGoTop = false;
		mario->isAutoGoLeft = false;
		mario->isAutoGoRight = false;
		break;
	}
}

void CMapSceneKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = CMario::GetInstance();
	//CMario* mario = ((CSenceMap*)scence)->GetPlayer();
	
}