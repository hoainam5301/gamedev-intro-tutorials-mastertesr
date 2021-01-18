#include <iostream>
#include <fstream>
#include "Game.h"
#include "PlayScence.h"
//#include "Utils.h"
//#include "Textures.h"
//#include "Sprites.h"


//

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TILEMAP 7

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_ITEMS	8
#define OBJECT_TYPE_WEAPON  9
#define OBJECT_TYPE_PORTAL	50
#define OBJECT_TYPE_FLOOR   4
#define OBJECT_TYPE_COLOR_BOX	5
#define OBJECT_TYPE_PIRANHA_FLOWER_SHOOT 6
#define OBJECT_TYPE_BROKEN_BRICK	7
#define OBJECT_TYPE_COIN			10
#define OBJECT_TYPE_PIRANHA_BITE 11
#define OBJECT_TYPE_MOONEY_EFFECT 12
#define OBJECT_TYPE_PIPE		13
#define OBJECT_TYPE_MOVING_WOOD 14
#define OBJECT_TYPE_BOOMERANG_BROTHER 15


#define MAX_SCENE_LINE 1024

void CPlayScene::_ParseSection_TileMap(string line)
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
	gridObjIdle = new CGrid();
	gridObjIdle->Resize();
	
	gridObjMove = new CGrid();
	gridObjMove->Resize();						///toi dayyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
	

	//int texID = atoi(tokens[0].c_str());
	map = new TileMap(ID, file_texture.c_str(), file_path.c_str(), row_on_textures, col_on_textures, row_on_tile_map, col_on_tile_map,  tile_width, tile_height);

}
void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
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

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
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

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	// General object setup
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	CGameObject* obj = NULL;
	//CMario* Player = NULL;
	//CGameObject* brokenBrick = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		/*if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}*/		
		/*obj=CMario::GetInstance();
		player = dynamic_cast<CMario*> (obj);
		player = static_cast<CMario*>(obj);*/

		player = CMario::GetInstance();
		player->SetPosition(x, y);
		player->SetAnimationSet(ani_set);
		//listObjMove.push_back(player);
		
		statusBar = new StatusBar(player);
		//DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA:
	{
		obj = new CGoomba(player);
		obj->id_goomba = atoi(tokens[4].c_str());
		listObjMove.push_back(obj);
		//DebugOut(L"id gooombaaaaaa %d \n",obj->id_goomba);
	}break;
	case OBJECT_TYPE_ITEMS:
	{
		obj = new CItems(x, y);
		//items = (CItems*)obj;
	}break;
	case OBJECT_TYPE_BOOMERANG_BROTHER:
	{
		obj = new CBoomerangBrother(player);
		listObjMove.push_back(obj);
	}
	break;
	case OBJECT_TYPE_MOONEY_EFFECT:
	{
		obj = new CMonneyEffect(); 
		listObjMove.push_back(obj);
		break;
	}
	case OBJECT_TYPE_MOVING_WOOD:
	{
		float width = atof(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		obj = new CMovingWood(width,height,player);
		listMovingWood.push_back(obj);
		break;
	}
	case OBJECT_TYPE_WEAPON:
	{
		obj = new CWeapon(x, y, player->nx);		
	}
	case OBJECT_TYPE_BRICK:
	{		
		obj = new CBrick(x, y);
		obj->id_brick_items = atoi(tokens[4].c_str());	
		listObjMove.push_back(obj);
	}
	break;
	case OBJECT_TYPE_KOOPAS: 
	{
		int id_koopa= atoi(tokens[4].c_str());
		int hasWing = atoi(tokens[5].c_str());
		obj = new CKoopas(player,id_koopa, hasWing);
		listObjMove.push_back(obj);
		break;
	}
	case OBJECT_TYPE_PIRANHA_FLOWER_SHOOT:
	{
		obj = new CGiantPiranhaPlant(player); 
		obj->id_giantpiranha = atoi(tokens[4].c_str());
		listObjMove.push_back(obj);
		break;
	}
	case OBJECT_TYPE_BROKEN_BRICK:
	{
		int id_state = atoi(tokens[4].c_str());
		obj = new CBrokenBrick(id_state); 
		//obj->id_broken_state= atoi(tokens[4].c_str());
		listObjMove.push_back(obj);
		break;
	}
	case OBJECT_TYPE_COIN:
	{
		obj = new CCoin();
		//listObjMove.push_back(obj);
		listCoin.push_back(obj);
		break;
	}
	case OBJECT_TYPE_PIRANHA_BITE:
	{
		obj = new CGiantPiranhaPlantBite(player); 
		listObjMove.push_back(obj);
		break;
	}
	case OBJECT_TYPE_FLOOR:
	{
		float width = atof(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		obj = new CFloor(width, height);
		if (obj)
		{
			obj->SetPosition(x, y);
			obj->SetAnimationSet(ani_set);
			//objects.push_back(obj);
			listObjIdle.push_back(obj);
		}
		break;
	}
	case OBJECT_TYPE_PIPE:
	{
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());
		int pipeType = atoi(tokens[6].c_str());
		obj = new CPipe(width, height,pipeType);
		listObjIdle.push_back(obj);
		break;
	}
	case OBJECT_TYPE_COLOR_BOX:
	{
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());
		obj = new CColorBox(width, height);
		listObjIdle.push_back(obj);
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
		listObjIdle.push_back(obj);
	}
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
int CPlayScene::RandomItems()
{
	if (player->level == MARIO_LEVEL_SMALL)
		return Mushroom;
	else if (player->level == MARIO_LEVEL_BIG)
		return Tree_Leaf;
	else if (player->level == MARIO_RACCOON)
		return FIRE_FLOWER;
	else if (player->level == MARIO_FIRE)
		return Tree_Leaf;		
}
void CPlayScene::Load()
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

void CPlayScene::SetCamSpeedY(ULONGLONG dt)
{
	float camSpeedY = 0.0f;
	if (camY == 0)
		camY = 200;
	if (player->inHiddenArea && !player->inMapTwo)
	{
		camY = 400;
	}
	else if (!player->inHiddenArea)
	{
		if (camY == 400 || camY == 0)
			camY = 200;
		if (player->is_Grounded)
		{
			camSpeedY = 0;
			camY = 200;
			if (player->topOfMario < camY)   //dung tren cuc gach tren cao va cloud brick
			{
				camY = player->topOfMario - 170;
				player->standOnCloudBrick = true;
				player->inHighArea = true;
				//DebugOut(L"pppppppppppppp \n");
			}
			if (player->topOfMario < 100 && player->standOnPipe)
			{
				camY = player->topOfMario - 50;
				//DebugOut(L"lllllllllllllllll \n");
			}
			goto SetCam;
		}
		if (player->isFlying)
		{
			//camSpeedY = player->vy;

			camY = player->y -100.0f;
			
			//DebugOut(L"aaaaaaaaaa \n");
		}
		else if (player->inHighArea)
		{
			if (player->standOnCloudBrick)
			{
				camSpeedY = player->vy;
				//camY = player->topOfMario - 100;
				//DebugOut(L"bbbbbbbbb \n");
			}
			else if (player->isJumping)
			{
				//camSpeedY = player->vy;
				camY = player->topOfMario - 100;
			//	DebugOut(L"cccccccc \n");
			}
			else if (player->topOfMario > 200 && !player->isJumping)
			{
				//DebugOut(L"eeeeeee \n");
				camY = 200;
			}
		}
		else if (player->isJumping)
		{
			//DebugOut(L"%.4f, %.4f\n", camY);
			if (player->topOfMario < camY+5 )
			{
				camSpeedY = player->vy;
			}
			else if (player->isFalling)
			{
				camY = player->y-20;
			}
		}

	SetCam:
		camY += camSpeedY * dt;
		if (camY <= 0 || camY > 200)
			return;
	}
	CGame::GetInstance()->SetCamPosY(round(camY));
}	

void CPlayScene::Update(ULONGLONG dt)
{
	gridObjIdle->MakeObjOutOfCam(listMovingWood);
	gridObjMove->MakeObjOutOfCam(listObjMove);
	if (GetTickCount64() - timeTranformation > 5000)
	{
		for (int i = 0; i < listObjMove.size(); i++)
		{
			if (dynamic_cast<CBrokenBrick*>(listObjMove[i]) && !listObjMove[i]->isdone)
			{
				CBrokenBrick* broken = dynamic_cast<CBrokenBrick*>(listObjMove[i]);
				broken->tranformation = false;
			}
		}
	}


	InsertObjToGrid();
	
	vector<LPGAMEOBJECT> coObjects;
	/*for (int i = 0; i < listObjIdle.size(); i++)
		listObjIdle[i]->Update(dt);*/
	if (player->endGame && GetTickCount64() - player->readySwitchOutGame > 4000 && player->readySwitchOutGame!=0)
	{
		
		CGame::GetInstance()->SwitchScene(2);
		player->inSelectMap = true;
		player->SetPosition(player->posXOfPortal, player->posYOfPortal);
		player->SetLevel(MARIO_LEVEL_SMALL);
		hasMakeRandomItemsEndGame = false;
		player->readySwitchOutGame = 0;
		player->endGame = false;
		CGame::GetInstance()->SetCamPos(0,0);
		player->vx = 0;
		player->vy = 0;
		
		return;
	}
	player->Update(dt, &objects/*,&listObjIdle*/);

	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		//objects[i]->Update(dt, &listObjIdle);
		objects[i]->Update(dt, &coObjects);
		LPGAMEOBJECT a = objects[i];

		if (dynamic_cast<CBrick*>(a) && a->id_brick_items == ID_GACH_RA_ITEMS)
		{
			CBrick* gach = dynamic_cast<CBrick*>(a);
			if (gach->bottom_coll == 1 && gach->created_item == 0 && gach->bouncing == 1)
			{
				gach->created_item = 1;
				if(player->level==MARIO_LEVEL_BIG)
					leafTree.push_back(MadeItems(gach->x, gach->y));
				else 
					listitems.push_back(MadeItems(gach->x, gach->y));
			}
			else if (gach->collWithKoopa && gach->created_item==0)
			{
				gach->created_item = 1;
				CItems* item = new CItems(gach->y, gach->x);
				item->SetPosition(gach->x, gach->y);
				item->id_items = Tree_Leaf;
				item->SetState(Tree_Leaf);				
				//listitems.push_back(item);
				leafTree.push_back(item);
			}
		}
		else if (dynamic_cast<CBrick*>(a) && a->id_brick_items == ID_GACH_RA_NHIEU_TIEN)
		{
			CBrick* gach = dynamic_cast<CBrick*>(a);

			if (gach->created_item == 0 && gach->bottom_coll == 1 && gach->bouncing == 1)
			{
				coin = new CCoin();
				if (gach->countMoneyHasMake >= 10)
				{
					gach->created_item = 1;
					gach->move = false;
				}
				else
				{
					gach->bottom_coll = 0;
					gach->created_item = 0;
					gach->move = false;
					gach->bouncing = 0;
				}
				//gach->created_item = 1;
				player->dola++;
				player->score += 100;
				coin->SetPosition(gach->x + 3, gach->y);
				coin->SetState(COIN_STATE_CREATED);
			}
		}
		else if (dynamic_cast<CBrick*>(a) && a->id_brick_items == ID_GACH_RA_LEAF_TREA)
		{
			CBrick* gach = dynamic_cast<CBrick*>(a);
			if (gach->created_item == 0 && gach->bottom_coll == 1 && gach->bouncing == 1)
			{
				gach->created_item = 1;
				CItems* item = new CItems(gach->y, gach->x);
				item->id_items = Tree_Leaf;
				item->SetState(Tree_Leaf);
				item->SetPosition(gach->x, gach->y - 16);
				leafTree.push_back(item);
			}
		}
		else if (dynamic_cast<CBrick*>(a) && a->id_brick_items == ID_GACH_RA_TIEN) 
		{
			CBrick* gach = dynamic_cast<CBrick*>(a);

			if (gach->created_item == 0 && gach->bottom_coll == 1 && gach->bouncing == 1)
			{
				coin = new CCoin();
				gach->created_item = 1;
				player->dola++;
				player->score += 100;
				coin->SetPosition(gach->x+3, gach->y);
				coin->SetState(COIN_STATE_CREATED);
			}
		}
		else if (dynamic_cast<CBrick*>(a) && a->id_brick_items == ID_GACH_SWITCH_P)
		{
			CBrick* gach = dynamic_cast<CBrick*>(a);
			if (gach->created_item == 0 && gach->hitByTail)
			{
				gach->created_item = 1;
				CItems* item = new CItems(gach->x, gach->y);
				item->id_items = SWITCH_P_ON;
				item->SetState(SWITCH_P_ON);
				item->SetPosition(gach->x, gach->y - 16);
				listitems.push_back(item);
			}	
			else if (gach->created_item == 0 && gach->bottom_coll == 1 && gach->bouncing == 1)
			{
					gach->created_item = 1;
					CItems* item = new CItems(gach->x, gach->y);
					item->id_items = SWITCH_P_ON;
					item->SetState(SWITCH_P_ON);
					item->SetPosition(gach->x, gach->y - 16);
					listitems.push_back(item);
			}
			
		}
		else if (dynamic_cast<CBrick*>(a) && a->id_brick_items == ID_GACH_BONUS)
		{
			CBrick* gach = dynamic_cast<CBrick*>(a);

			if (gach->created_item == 0 && gach->bottom_coll == 1 && gach->bouncing == 1)
			{
				gach->created_item=1;
				CItems* item = new CItems(gach->x, gach->y);
				item->id_items = GREEN_MUSHROOM;
				item->SetState(GREEN_MUSHROOM);
				item->SetPosition(gach->x, gach->y);
				listitems.push_back(item);
			}
		}
		if (coin != NULL)
		{
			coin->Update(dt, &coObjects);
			for (int i = 0; i < coin->listEffect.size(); i++)
			{
				coin->listEffect[i]->Update(dt,&coObjects);
				if (coin->listEffect[i]->isdone)
				{
					coin->isdone = true;					
				}
			}
		}
	}
		
	for (int i = 0; i < listCoin.size(); i++)
	{
		listCoin[i]->Update(dt, &coObjects);
	}
	/*if (player->x > 2256)
		MakeItemEndGame();*/
	for (int i = 0; i < listitems.size(); i++)
	{
		if (listitems[i]->GetState() == SWITCH_P_OFF)
		{
			LPGAMEOBJECT a = listitems[i];
			CItems* item = dynamic_cast<CItems*>(a);
			if (!item->hasPress)
			{
				for (int i = 0; i < objects.size(); i++)
				{
					LPGAMEOBJECT broken = objects[i];
					if (dynamic_cast<CBrokenBrick*>(broken))
					{
						CBrokenBrick* brokenbrick = dynamic_cast<CBrokenBrick*>(broken);
						if (!brokenbrick->isDestroyed)
						{
							brokenbrick->tranformation = true;
							timeTranformation = GetTickCount64();
							CCoin* coin = new CCoin();
							coin->SetPosition(brokenbrick->x, brokenbrick->y);
							coin->SetState(COIN_STATE_NO_ROTATE);
							listcoin.push_back(coin);
						}						
					}
				}
				
				//for (int j = 0; j < listCoin.size(); j++)
				//{
				//	LPGAMEOBJECT Coin = listCoin[j];
				//	if (dynamic_cast<CCoin*>(Coin))
				//	{
				//		CCoin * coin = dynamic_cast<CCoin*>(Coin);
				//		if (!coin->isdone /*&& coin->tranformation*/)
				//		{
				//			coin->tranformation = true;
				//			timeTranformation = GetTickCount64();
				//			CBrokenBrick * brokenbrick= new CBrokenBrick (1);
				//			brokenbrick->SetPosition(coin->x, coin->y);
				//			brokenbrick->SetState(STATE_BRICK_NORMAL);
				//			listcoin.push_back(brokenbrick);
				//		}
				//	}
				//}
				item->hasPress = true;
			}			
		}
		LPGAMEOBJECT a = listitems[i];
		CItems* item = dynamic_cast<CItems*>(a);
		if (item->makeItemFly)
		{
			CItems* itemfly = new CItems(item->x, item->y);
			itemfly->id_items = START_FLY_UP;
			itemfly->SetState(START_FLY_UP);
			itemfly->SetPosition(item->x, item->y);
			listitems.push_back(itemfly);
			item->makeItemFly = false;	
			player->countItemsEndGame++;
		}
	}

	if (GetTickCount64() - timeTranformation > 5000 && timeTranformation!=0)
	{
		for (int i = 0; i < objects.size(); i++)
		{
			LPGAMEOBJECT broken = objects[i];
			if (dynamic_cast<CBrokenBrick*>(broken))
			{
				CBrokenBrick* brokenbrick = dynamic_cast<CBrokenBrick*>(broken);
				if (!brokenbrick->isDestroyed)
				{				
					for (int j = 0; j < listcoin.size(); j++)
					{
						if (listcoin[j]->y == brokenbrick->y && listcoin[j]->isdone && listcoin[j]->x== brokenbrick->x)
							brokenbrick->isdone = true;
						else
							brokenbrick->tranformation = false;
					}
				}
			}
		}
		//for (int j = 0; j < listcoin.size(); j++)
		//{
		//	LPGAMEOBJECT broken = listcoin[j];
		//	if (dynamic_cast<CBrokenBrick*>(broken))
		//	{
		//		CBrokenBrick* brokenbrick = dynamic_cast<CBrokenBrick*>(broken);
		//		if (!brokenbrick->isdone)
		//		{				
		//			for (int j = 0; j < listCoin.size(); j++)
		//			{
		//				if (listCoin[j]->y == brokenbrick->y/* && listCoin[j]->isdone &&*/&& listCoin[j]->x== brokenbrick->x)
		//				{
		//					CCoin* Coin = dynamic_cast<CCoin*>(listCoin[j]);
		//					Coin->tranformation = false;
		//				}
		//				else
		//				{
		//					CCoin* Coin = dynamic_cast<CCoin*>(listCoin[j]);
		//					Coin->tranformation = false;
		//				}

		//			}
		//		}
		//	}
		//}
		listcoin.clear();
	}

	//item end game can nghi cach lam lai 
	if (!player->inMapTwo)  // chia cat 2 map
	{
		if (player->x > 2256 && !hasMakeRandomItemsEndGame)
		{
			CItems* item = new CItems(2689, 338);
			item->id_items = ITEMS_END_GAME;
			item->SetState(ITEMS_END_GAME);
			item->SetPosition(2689, 338);
			listitems.push_back(item);
			hasMakeRandomItemsEndGame = true;
		}
	}
	else
	{
		if (player->x > 2193 && !hasMakeRandomItemsEndGame)
		{
			CItems* item = new CItems(2449, 338);
			item->id_items = ITEMS_END_GAME;
			item->SetState(ITEMS_END_GAME);
			item->SetPosition(2449, 338);
			listitems.push_back(item);
			hasMakeRandomItemsEndGame = true;
		}
	}

	if (player->use_Weapon && !player->hasFight)
	{
		if (player->nx > 0)
			listweapon.push_back(MadeWeapon(player->x + 10, player->y + 6, player->nx));
		else
			listweapon.push_back(MadeWeapon(player->x +5, player->y + 6, player->nx));
		player->hasFight = true; //mario da ban dan roi
	}
	if (player->level == MARIO_RACCOON && (player->GetState() == MARIO_RACCOON_STATE_FIGHT_IDLE_LEFT || player->GetState() == MARIO_RACCOON_STATE_FIGHT_IDLE_RIGHT))
	{
		if (player->nx > 0)
			tail->SetPosition(player->x + 15, player->y + 18);
		else
			tail->SetPosition(player->x - 2, player->y + 18);
		tail->isFighting = true;
		tail->Update(dt, &coObjects);
	}
	else
		tail->isFighting = false;

	player->Collision_items(&listitems);
	player->Collision_coin(&listcoin);
	player->Collision_coin(&listCoin);
	player->Collision_items(&leafTree);
	if (listweapon.size() != 0)
	{
		if (listweapon.size() >= 2)
			player->loadFireball = true;
		for (int i = 0; i < listweapon.size(); i++)
		{
			listweapon[i]->Update(dt, &coObjects);
		}
	}
	for (int i = 0; i < listitems.size(); i++)
		listitems[i]->Update(dt, &coObjects);
	for (int i = 0; i < listcoin.size(); i++)
		listcoin[i]->Update(dt, &coObjects);

	for (int i = 0; i < leafTree.size(); i++)
		leafTree[i]->Update(dt, &coObjects);

	if (listweapon.size() != 0)
	{
		for (int i = 0; i < listweapon.size(); i++)
		{
			if (listweapon[i]->isExplode)
			{
				listweapon.erase(listweapon.begin() + i);
			}
		}

	}
	else
		player->loadFireball = false;
	
	if (player == NULL) return;
	
	// Update camera to follow mario
	
	if (!player->inMapTwo)
	{
		cx = player->x - (SCREEN_WIDTH / 4);
		if (player->x > (SCREEN_WIDTH / 4) && player->x + (SCREEN_WIDTH / 4) < 2832)
		{
			cx = player->x - (SCREEN_WIDTH / 4);
			CGame::GetInstance()->cam_x = round(cx);
		}
	}
	else
	{	

		camX += 0.03 * dt;
		if(player->x==2193)
			camX = player->x - (SCREEN_WIDTH / 4);
		/*if (camX > 0.1)
			camX = 0.1;*/
		if (camX > 1778 && camX<1800)
			camX = 1778.0f;
		else if (camX > 1800)
		{
			camX = player->x - (SCREEN_WIDTH / 4);
			
			if (player->x > (SCREEN_WIDTH / 4) && player->x + (SCREEN_WIDTH / 4) < 2528)
			{				
				camX = player->x - (SCREEN_WIDTH / 4);
				if (camX < 2048)
					camX = 2048;
				
				//CGame::GetInstance()->cam_x = round();
			}
			if (camX > 2290)
				camX = 2290;
		}
		//DebugOut(L"gia tri cua camX %f \n", camX);
		CGame::GetInstance()->SetCamPosX(1550.0);
		CGame::GetInstance()->SetCamPosX(round(camX));
		/*cx = player->x - (SCREEN_WIDTH / 4);
		if (player->x > (SCREEN_WIDTH / 4) && player->x + (SCREEN_WIDTH / 4) < 2832)
		{
			cx = player->x - (SCREEN_WIDTH / 4);
			CGame::GetInstance()->cam_x = round(cx);
		}*/
	}
	if (player->inMapTwo)
	{
		if (player->leftOfMario < CGame::GetInstance()->GetCamPosX())
		{
			player->vx = MARIO_WALKING_SPEED/2;
		}
	}
	SetCamSpeedY(dt);
	statusBar->Update(dt, CGame::GetInstance()->cam_x, CGame::GetInstance()->cam_y);
	
	gridObjMove->ResetGrid(listObjMove);
	gridObjIdle->ResetGrid(listMovingWood);
}

void CPlayScene::Render()
{	
	if (!player->inSelectMap)
	{
		map->Draw();
		/*for (int i = 0; i < listcoin.size(); i++)
		{
			listcoin[i]->Render();
		}*/
		if (player->endGame && GetTickCount64() - player->readySwitchOutGame > 1000 && player->readySwitchOutGame != 0 && GetTickCount64() - player->readySwitchOutGame < 4000)
			RenderTextEndGame();
		for (int i = 0; i < listCoin.size(); i++)
		{
			listCoin[i]->Render();
		}

		for (int i = 0; i < listweapon.size(); i++)
		{
			if (!listweapon[i]->isExplode)
				listweapon[i]->Render();
		}
		for (int i = 0; i < listcoin.size(); i++)
			listcoin[i]->Render();
		for (int i = 0; i < listitems.size(); i++)
			listitems[i]->Render();

		for (int i = objects.size() - 1; i >= 0; i--)
			objects[i]->Render();
		for (int i = 0; i < leafTree.size(); i++)
			leafTree[i]->Render();

		if (tail != NULL)
			tail->Render();
		if (coin != NULL)
			coin->Render();

		if (player->turnOffLight)
		{
			if (GetTickCount64() - player->waitGetOutOfPipe < 200 && player->countRender < 6)
			{
				CAnimations::GetInstance()->Get(774)->Render(1000, 0, 200);
				player->countRender++;
			}
		}

		player->Render();
		for (int i = 0; i < listObjIdle.size(); i++)
			listObjIdle[i]->Render();
		//CAnimations::GetInstance()->Get(794)->Render(60, 300);
		statusBar->Render();
	}
}

void CPlayScene::InsertObjToGrid()
{
	objects.clear();
	listAllObjIdle.clear();
	listAllObjMove.clear();
	//gridObjIdle->GetGrid(listObjIdle);
	for (UINT i = 0; i < listObjIdle.size(); i++)
	{
		objects.push_back(listObjIdle[i]);
	}
	gridObjIdle->GetGrid(listAllObjIdle);
	for (UINT i = 0; i < listAllObjIdle.size(); i++)
	{
		objects.push_back(listAllObjIdle[i]);
	}
	
	gridObjMove->GetGrid(listAllObjMove);	
	for(UINT i = 0; i < listAllObjMove.size(); i++)
	{
		objects.push_back(listAllObjMove[i]);
	}
}
void CPlayScene::MakeItemEndGame()
{
	if (!player->inMapTwo)  // chia cat 2 map
	{
		CItems* item = new CItems(2689, 338);
		item->id_items = ITEMS_END_GAME;
		item->SetState(ITEMS_END_GAME);
		item->SetPosition(2689, 338);
		listitems.push_back(item);
	}
}
void CPlayScene::RenderTextEndGame()
{
	if (!player->inMapTwo)
	{
		for (int i = 0; i < lisstTextClear.size(); i++)
		{
			if (lisstTextClear[i] == ' ')
				continue;
			int numb;
			for (int j = 0; j < 26; j++)
			{
				numb = 65 + j;
				if (lisstTextClear[i] == numb)
				{
					text = CSprites::GetInstance()->Get(2100 + (j));
					break;
				}
			}
			text->Draw(2650 + (i * 8) + X, 260);
		}

		for (int i = 0; i < listTextEndGame.size(); i++)
		{
			if (listTextEndGame[i] == ' ')
				continue;
			int numb;
			for (int j = 0; j < 26; j++)
			{

				numb = 65 + j;
				if (listTextEndGame[i] == numb)
				{
					text = CSprites::GetInstance()->Get(2100 + (j));
					break;
				}
			}
			text->Draw(2644 + (i * 8) + X, 280);
		}

		CSprites::GetInstance()->Get(2067)->Draw(2777, 260);
		CSprites::GetInstance()->Get(2065)->Draw(2781, 266);
	}
	else
	{
		for (int i = 0; i < lisstTextClear.size(); i++)
		{
			if (lisstTextClear[i] == ' ')
				continue;
			int numb;
			for (int j = 0; j < 26; j++)
			{
				numb = 65 + j;
				if (lisstTextClear[i] == numb)
				{
					text = CSprites::GetInstance()->Get(2100 + (j));
					break;
				}
			}
			text->Draw(2410 + (i * 8) + X, 260);
		}

		for (int i = 0; i < listTextEndGame.size(); i++)
		{
			if (listTextEndGame[i] == ' ')
				continue;
			int numb;
			for (int j = 0; j < 26; j++)
			{

				numb = 65 + j;
				if (listTextEndGame[i] == numb)
				{
					text = CSprites::GetInstance()->Get(2100 + (j));
					break;
				}
			}
			text->Draw(2404 + (i * 8) + X, 280);
		}

		CSprites::GetInstance()->Get(2067)->Draw(2537, 260);
		CSprites::GetInstance()->Get(2065)->Draw(2541, 266);
	}
	
}
/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	listAllObjIdle.clear();
	listAllObjMove.clear();
	listObjIdle.clear();
	listObjMove.clear();
	listMovingWood.clear();
	listCoin.clear();
	listcoin.clear();
	leafTree.clear();
	listitems.clear();
	listweapon.clear();

	objects.clear();

	gridObjIdle = NULL;
	gridObjMove = NULL;
	//player = NULL;

}
void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CGame* game = CGame::GetInstance();
	//CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	CMario* mario = CMario::GetInstance();
	switch (KeyCode)
	{
	case DIK_S:
		
		if (mario->is_Grounded)
		{
			mario->isJumping = false;
		}
		if (mario->level == MARIO_RACCOON)
		{
			if (mario->isMaxSpeed && !mario->Firstspaceup)
			{
				//DebugOut(L"huong nx %d \n", mario->nx);
				if (mario->nx > 0)
					mario->SetState(MARIO_RACCOON_STATE_KEEP_FLYING_RIGHT);
				else
					mario->SetState(MARIO_RACCOON_STATE_KEEP_FLYING_LEFT);		
				mario->isFlying = true;
				mario->Firstspaceup = false;
				return;
			}			
			
		}
		if (mario->isJumping)
		{
			mario->isFalling = true;
			//mario->inHighArea = true;
		}
		if (mario->isFalling)
		{		
			if (mario->level == MARIO_RACCOON && mario->isJumping  && !mario->isFlying && !mario->Firstspaceup )																			//neu khong phai la lan tha phim space dau tien thi ao trang thai quay duoi 
			{				
				if (mario->nx > 0)
					mario->SetState(MARIO_RACCOON_STATE_FALLING_ROCK_TAIL_RIGHT);
				else
					mario->SetState(MARIO_RACCOON_STATE_FALLING_ROCK_TAIL_LEFT);
			}
			else
			{
				mario->vy = mario->vy + MARIO_GRAVITY * 10 * mario->dt; //nhan space toi dau thi nhay toi do
				mario->Firstspaceup = false;
			}
		}
		
		//mario->vy = mario->vy + MARIO_GRAVITY * 15 * mario->dt;
		break;
	case DIK_A:
		mario->isRunning = false;
		if (mario->level == MARIO_FIRE)
		{

			mario->use_Weapon = false;
			mario->hasFight = false;			//mario chua ban dan
		}

		if (mario->isHolding)
		{
			if (mario->nx > 0)
			{
				if (mario->level == MARIO_LEVEL_BIG)
					mario->SetState(MARIO_BIG_STATE_KICK_RIGHT);
				else if (mario->level == MARIO_LEVEL_SMALL)
					mario->SetState(MARIO_SMALL_STATE_KICK_RIGHT);
				else if (mario->level == MARIO_RACCOON)
					mario->SetState(MARIO_RACCOON_STATE_KICK_RIGHT);
				else if (mario->level == MARIO_FIRE)
					mario->SetState(MARIO_FIRE_STATE_KICK_RIGHT);

			}
			else
			{
				if (mario->level == MARIO_LEVEL_BIG)
					mario->SetState(MARIO_BIG_STATE_KICK_LEFT);
				else if (mario->level == MARIO_LEVEL_SMALL)
					mario->SetState(MARIO_SMALL_STATE_KICK_LEFT);
				else if (mario->level == MARIO_RACCOON)
					mario->SetState(MARIO_RACCOON_STATE_KICK_LEFT);
				else if (mario->level == MARIO_FIRE)
					mario->SetState(MARIO_FIRE_STATE_KICK_LEFT);
			}
			mario->isHolding = false;
		}
		break;
		//case DIK_Q:
		//	mario->use_Weapon = false;
		//	mario->isdone = false;
		//	break;
	case DIK_RIGHT:
	case DIK_LEFT:
	/*	if (mario->isFlying)
			mario->isFlying = false;*/
		break;

	}
}
void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	//DebugOut(L"aaaaaa \n");
	CGame* game = CGame::GetInstance();
	//CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	CMario* mario = CMario::GetInstance();
 	switch (KeyCode)
	{
	case DIK_S:
		mario->isStandOnMovingWood = false;
		if (mario->isFalling)
		{
			if (mario->level == MARIO_RACCOON && mario->isJumping && !mario->isFlying)
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_RACCOON_STATE_FALLING_ROCK_TAIL_RIGHT);
				else
					mario->SetState(MARIO_RACCOON_STATE_FALLING_ROCK_TAIL_LEFT);
				//mario->vy = (-0.0006 * 1.2 *mario-> dt);									
			}
		}
		if (mario->isJumping || mario->getUpInPipe)
			return;
		mario->is_Grounded = false;
		mario->standOnCloudBrick = false;
		//mario->canNotWalking = false;
		mario->isJumping = true;
		//mario->firstspaceup = true;
		if (!mario->isFlying) 
		{
			if (mario->level == MARIO_LEVEL_BIG)
			{
				if (mario->isMaxSpeed && !mario->isFlying)
				{
					//DebugOut(L"huong nx %d \n", mario->nx);
					if (mario->nx > 0)
						mario->SetState(MARIO_BIG_STATE_JUMP_RIGHT_WHEN_MAX_SPEED);
					else
						mario->SetState(MARIO_BIG_STATE_JUMP_LEFT_WHEN_MAX_SPEED);
					mario->isFlying = true;
					mario->isSitting = false;
				}
				else
				{
					if (mario->nx > 0)
						mario->SetState(MARIO_BIG_STATE_JUMP_RIGHT);
					else
						mario->SetState(MARIO_BIG_STATE_JUMP_LEFT);
				}
			}
			else if (mario->level == MARIO_LEVEL_SMALL)
			{
				if (mario->isMaxSpeed && !mario->isFlying)
				{
					//DebugOut(L"huong nx %d \n", mario->nx);
					if (mario->nx > 0)
						mario->SetState(MARIO_SMALL_STATE_JUMP_RIGHT_WHEN_MAX_SPEED);
					else
						mario->SetState(MARIO_SMALL_STATE_JUMP_LEFT_WHEN_MAX_SPEED);
					mario->isFlying = true;
					mario->isSitting = false;
				}
				else
				{
					if (mario->nx > 0)
						mario->SetState(MARIO_SMALL_STATE_JUMP_RIGHT);
					else
						mario->SetState(MARIO_SMALL_STATE_JUMP_LEFT);
				}
			}
			else if (mario->level == MARIO_RACCOON)
			{
  				if (mario->isMaxSpeed && !mario->isFlying)
				{
					//DebugOut(L"huong nx %d \n", mario->nx);
					if (mario->nx > 0)
						mario->SetState(MARIO_RACCOON_STATE_FLYING_RIGHT);
					else
    					mario->SetState(MARIO_RACCOON_STATE_FLYING_LEFT);
 					mario->isFlying = true;
					mario->isSitting = false;
				}
				else 
				{
					if (mario->nx > 0)
						mario->SetState(MARIO_RACCOON_STATE_JUMP_RIGHT);
					else
						mario->SetState(MARIO_RACCOON_STATE_JUMP_LEFT);
				}
			}
			else if (mario->level == MARIO_FIRE)
			{
				if (mario->isMaxSpeed && !mario->isFlying)
				{
					//DebugOut(L"huong nx %d \n", mario->nx);
					if (mario->nx > 0)
						mario->SetState(MARIO_FIRE_STATE_JUMP_RIGHT_WHEN_MAX_SPEED);
					else
						mario->SetState(MARIO_FIRE_STATE_JUMP_LEFT_WHEN_MAX_SPEED);
					mario->isFlying = true;
					mario->isSitting = false;
				}
				else
				{
					if (mario->nx > 0)
					{
						mario->SetState(MARIO_FIRE_STATE_JUMP_RIGHT);
						//DebugOut(L"bbbbbb");
					}
					else
						mario->SetState(MARIO_FIRE_STATE_JUMP_LEFT);
				}
			}
  		mario->vy = -MARIO_JUMP_SPEED_Y;		
		}
		//DebugOut(L"gia tri is high jump %d \n", mario->isHigh_Jumping);
		break;
	case DIK_Z:
		mario->Reset();
		CGame::GetInstance()->cam_x = 0;
		break;
	case DIK_Q:
		if (mario->level == MARIO_LEVEL_SMALL)
			mario->SetState(MARIO_SMALL_STATE_KICK_RIGHT);
		else if (mario->level == MARIO_LEVEL_BIG)
			mario->SetState(MARIO_BIG_STATE_KICK_RIGHT);
		else if (mario->level == MARIO_RACCOON)
			mario->SetState(MARIO_RACCOON_STATE_KICK_RIGHT);
		else if (mario->level == MARIO_FIRE)
			mario->SetState(MARIO_FIRE_STATE_KICK_RIGHT);
		break;
	case DIK_W:
		if (mario->level == MARIO_LEVEL_SMALL)
			mario->SetState(MARIO_SMALL_STATE_KICK_LEFT);
		else if (mario->level == MARIO_LEVEL_BIG)
			mario->SetState(MARIO_BIG_STATE_KICK_LEFT);
		else if (mario->level == MARIO_RACCOON)
			mario->SetState(MARIO_RACCOON_STATE_KICK_LEFT);
		else if (mario->level == MARIO_FIRE)
			mario->SetState(MARIO_FIRE_STATE_KICK_LEFT);
		break;
	case DIK_1:
		mario->level = MARIO_LEVEL_SMALL;
		break;
	case DIK_2:
		mario->level = MARIO_LEVEL_BIG;
		break;
	case DIK_3:
		mario->level = MARIO_RACCOON;
		break;
	case DIK_4:
		mario->level = MARIO_FIRE;
		break;
	case DIK_8:
		mario->SetPosition(1900,300);
		mario->SetLevel(MARIO_RACCOON);
		break;
	case DIK_9:
		//mario->SetLevel(MARIO_RACCOON);
		//mario->SetPosition(2256, 80);
		CGame::GetInstance()->SwitchScene(2);
		//DebugOut(L"gai tri X cua portal truoc do  %f  \n", mario->posXOfPortal);
		//DebugOut(L"gai tri Y cua portal truoc do  %f  \n", mario->posYOfPortal);
		mario->SetPosition(mario->posXOfPortal, mario->posYOfPortal);
		mario->SetLevel(MARIO_LEVEL_SMALL);
		mario->vx = 0;
		mario->vy = 0;
		//DebugOut(L"gai tri X cua mario truoc do  %f  \n", mario->posXOfPortal);
		//DebugOut(L"gai tri Y cua mario truoc do  %f  \n", mario->posYOfPortal);
		break;
	case DIK_A:
		
		if (mario->level == MARIO_FIRE)
		{
			if (mario->isSitting)
				break;
			if (mario->loadFireball)
				return;
			mario->use_Weapon = true;			
			if (mario->isJumping)
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_FIRE_STATE_FIGHT_WHILE_JUMPING_RIGHT);
				else
					mario->SetState(MARIO_FIRE_STATE_FIGHT_WHILE_JUMPING_LEFT);
			}
			else
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_FIRE_STATE_FIGHT_RIGHT);
				else
					mario->SetState(MARIO_FIRE_STATE_FIGHT_LEFT);
			}
		}
		if (mario->level == MARIO_RACCOON && !mario->isHolding)
		{
			if (mario->isSitting)
				break;
			if (mario->nx > 0)
				mario->SetState(MARIO_RACCOON_STATE_FIGHT_IDLE_RIGHT);
			else
				mario->SetState(MARIO_RACCOON_STATE_FIGHT_IDLE_LEFT);
		}
		break;
		//mario->isHolding = true;

	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	//CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	CMario* mario = CMario::GetInstance();
	// disable control key when Mario die 

	if (mario->GetState() == MARIO_ANI_DIE)
		return;
	if (mario->isWaitingForAni)
		return;
	if (mario->canNotWalking)
		return;
	if (game->IsKeyDown(DIK_A))
	{
	
		if (mario->is_Grounded)
		{
			if (mario->level == MARIO_LEVEL_BIG)
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_BIG_STATE_RUNNING_RIGHT);
				else
					mario->SetState(MARIO_BIG_STATE_RUNNING_LEFT);
			}
			else if (mario->level == MARIO_LEVEL_SMALL)
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_SMALL_STATE_RUNNING_RIGHT);
				else
					mario->SetState(MARIO_SMALL_STATE_RUNNING_LEFT);
			}
			else if (mario->level == MARIO_RACCOON)
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_RACCOON_STATE_RUNNING_RIGHT);
				else
					mario->SetState(MARIO_RACCOON_STATE_RUNNING_LEFT);
			}
			else if (mario->level == MARIO_FIRE)
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_FIRE_STATE_RUNNING_RIGHT);
				else
					mario->SetState(MARIO_FIRE_STATE_RUNNING_LEFT);
			}
		}
	}	
	if (game->IsKeyDown(DIK_UP))
	{
		if (game->IsKeyDown(DIK_S))
		{
			if (mario->leftOfMario > 2321 && mario->leftOfMario < 2333)
			{
				mario->atEndOfPipeY = 464;
				mario->getUpInPipe = true;					
				//mario->vy = -0.01;
				mario->SetState(MARIO_RACCOON_STATE_MOVE_IN_PIPE);
			}
		}
	}
	if (game->IsKeyDown(DIK_RIGHT) && game->IsKeyDown(DIK_LEFT))
	{
		mario->SetState(MARIO_STATE_IDLE);
	}
	else if (game->IsKeyDown(DIK_RIGHT))
	{
		if (mario->level == MARIO_LEVEL_BIG)
			mario->SetState(MARIO_BIG_STATE_WALKING_RIGHT);
		else if (mario->level == MARIO_LEVEL_SMALL)
			mario->SetState(MARIO_SMALL_STATE_WALKING_RIGHT);
		else if (mario->level == MARIO_RACCOON)
			mario->SetState(MARIO_RACCOON_STATE_WALK_RIGHT);
		else if (mario->level == MARIO_FIRE)
			mario->SetState(MARIO_FIRE_STATE_WALK_RIGHT);
		mario->lastnx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{		
		if (mario->level == MARIO_LEVEL_BIG)
			mario->SetState(MARIO_BIG_STATE_WALKING_LEFT);
		else if (mario->level == MARIO_LEVEL_SMALL)
			mario->SetState(MARIO_SMALL_STATE_WALKING_LEFT);
		else if (mario->level == MARIO_RACCOON)			
			mario->SetState(MARIO_RACCOON_STATE_WALK_LEFT);
		else if (mario->level == MARIO_FIRE)
			mario->SetState(MARIO_FIRE_STATE_WALK_LEFT);
		mario->lastnx = -1;
	}
	else if (game->IsKeyDown(DIK_DOWN) && !mario->isHolding && mario->level!=MARIO_LEVEL_SMALL && !mario->isFlying) // giu koopas ko ngoi dc
	{
		if (mario->isFlying)
			return;
		if (mario->level == MARIO_LEVEL_BIG)
		{
			if (mario->nx > 0)
				mario->SetState(MARIO_BIG_STATE_SITTING_RIGHT);
			else
				mario->SetState(MARIO_BIG_STATE_SITTING_LEFT);
		}
		else if (mario->level == MARIO_RACCOON)
		{
			if (mario->nx > 0)
				mario->SetState(MARIO_RACCOON_STATE_SITTING_RIGHT);
			else
				mario->SetState(MARIO_RACCOON_STATE_SITTING_LEFT);
		}
		else if (mario->level == MARIO_FIRE)
		{
			if (mario->nx > 0)
				mario->SetState(MARIO_FIRE_STATE_SITTING_RIGHT);
			else
				mario->SetState(MARIO_FIRE_STATE_SITTING_LEFT);
		}
		if (game->IsKeyDown(DIK_UP) && game->IsKeyDown(DIK_DOWN) && !mario->canNotWalking)
		{
			mario->SetState(MARIO_STATE_IDLE);
			mario->canNotWalking = true;
		}
	}
	else
	{
		if (mario->isFlying)
			return;
		if(mario->isHolding) //de cho mario khi dang be rua hien animation be rua thay vi dung yen(Can toi uu)
			if (mario->nx > 0)
			{				
				if (mario->level == MARIO_LEVEL_SMALL)
					mario->state = MARIO_SMALL_STATE_HOLDING_TURTLE_IDLE_RIGHT;
				else if (mario->level == MARIO_LEVEL_BIG)
					mario->state = MARIO_BIG_STATE_HOLDING_TURTLE_IDLE_RIGHT;
				else if (mario->level == MARIO_RACCOON)
					mario->state = MARIO_RACCOON_STATE_HOLDING_TURTLE_IDLE_RIGHT;
				else if (mario->level == MARIO_FIRE)
					mario->state = MARIO_BIG_STATE_HOLDING_TURTLE_IDLE_RIGHT;
			}
			else
			{
				if (mario->level == MARIO_LEVEL_SMALL)
					mario->state = MARIO_SMALL_STATE_HOLDING_TURTLE_IDLE_LEFT;
				else if (mario->level == MARIO_LEVEL_BIG)
					mario->state = MARIO_BIG_STATE_HOLDING_TURTLE_IDLE_LEFT;
				else if (mario->level == MARIO_RACCOON)
					mario->state = MARIO_RACCOON_STATE_HOLDING_TURTLE_IDLE_LEFT;
				else if (mario->level == MARIO_FIRE)
					mario->state = MARIO_FIRE_STATE_HOLDING_TURTLE_IDLE_LEFT;
			}
		if (mario->isJumping)
		{
			if (mario->isSitting)
			{
				if (mario->level == MARIO_LEVEL_BIG)
				{
					if (mario->nx > 0)
						mario->state = MARIO_BIG_STATE_SITTING_RIGHT;
					else
						mario->state = MARIO_BIG_STATE_SITTING_LEFT;
				}				
				else if (mario->level == MARIO_RACCOON)
				{
					if (mario->nx > 0)
						mario->state = MARIO_RACCOON_STATE_SITTING_RIGHT;
					else
						mario->state = MARIO_RACCOON_STATE_SITTING_LEFT;
				}
				else if (mario->level == MARIO_FIRE)
				{
					if (mario->nx > 0)
						mario->state = MARIO_FIRE_STATE_SITTING_RIGHT;
					else
						mario->state = MARIO_FIRE_STATE_SITTING_LEFT;
				}
				return;
			}
			if (mario->vy < 0) // de mario roi va nhay len co dang hang
			{
				if (mario->level == MARIO_LEVEL_BIG)
				{
					if (mario->nx > 0)
						mario->state = MARIO_BIG_STATE_JUMP_RIGHT;
					else
						mario->state = MARIO_BIG_STATE_JUMP_LEFT;
				}
				else if (mario->level == MARIO_LEVEL_SMALL)
				{
					if (mario->nx > 0)
						mario->state = MARIO_SMALL_STATE_JUMP_RIGHT;
					else
						mario->state = MARIO_SMALL_STATE_JUMP_LEFT;
				}
				else if (mario->level == MARIO_RACCOON)
				{
					if (mario->nx > 0)
						mario->state = MARIO_RACCOON_STATE_JUMP_RIGHT;
					else
						mario->state = MARIO_RACCOON_STATE_JUMP_LEFT;
				}
				else if (mario->level == MARIO_FIRE)
				{
					if (mario->nx > 0)
					{
						mario->state = MARIO_FIRE_STATE_JUMP_RIGHT;
						//DebugOut(L"ccccc");
					}
					else
						mario->state = MARIO_FIRE_STATE_JUMP_LEFT;
				}
			}
			else
			{
				if (mario->level == MARIO_LEVEL_BIG)
				{
					if (mario->nx > 0)
						mario->state = MARIO_BIG_STATE_FALLING_RIGHT;
					else
						mario->state = MARIO_BIG_STATE_FALLING_LEFT;
				}
				else if (mario->level == MARIO_RACCOON)
				{
					if (mario->nx > 0)
						mario->state = MARIO_RACCOON_STATE_FALLING_RIGHT;
					else
						mario->state = MARIO_RACCOON_STATE_FALLING_LEFT;
				}
				else if (mario->level == MARIO_FIRE)
				{
					if (mario->nx > 0)
						mario->state = MARIO_FIRE_STATE_FALLING_RIGHT;
					else
						mario->state = MARIO_FIRE_STATE_FALLING_LEFT;
				}
				
			}
			if (mario->isHolding)
			{
				if (mario->level == MARIO_LEVEL_BIG)
				{
					if (mario->nx > 0)
						mario->state = MARIO_BIG_STATE_HOLDING_TURTLE_JUMP_RIGHT;
					else
						mario->state = MARIO_BIG_STATE_HOLDING_TURTLE_JUMP_LEFT;
				}
				else if (mario->level == MARIO_LEVEL_SMALL)
				{
					if (mario->nx > 0)
						mario->state = MARIO_SMALL_STATE_HOLDING_TURTLE_JUMP_RIGHT;
					else
						mario->state = MARIO_SMALL_STATE_HOLDING_TURTLE_JUMP_LEFT;
				}
				else if (mario->level == MARIO_RACCOON)
				{
					if (mario->nx > 0)
						mario->state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_RIGHT;
					else
						mario->state = MARIO_RACCOON_STATE_HOLDING_TURTLE_JUMP_LEFT;
				}
				else if (mario->level == MARIO_FIRE)
				{
					if (mario->nx > 0)
						mario->state = MARIO_FIRE_STATE_HOLDING_TURTLE_JUMP_RIGHT;
					else
						mario->state = MARIO_FIRE_STATE_HOLDING_TURTLE_JUMP_LEFT;
				}
			}
			return;
		}
		mario->SetState(MARIO_STATE_IDLE);
		/*if(!mario->isJumping)
			mario->isSitting = false;*/
	}
	//DebugOut(L"bbbbbbbbbbbb %d\n", mario->isWaitingForAni);
}

//int CPlayScene::GetLeverPlayer() {
//	return player->GetLevel();
//}