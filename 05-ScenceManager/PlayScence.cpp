#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

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

#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_ITEMS	8
#define OBJECT_TYPE_WEAPON  9
#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


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

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA:
	{
		obj = new CGoomba();
	}break;
	case OBJECT_TYPE_ITEMS:
	{
		obj = new CItems(x, y);
		items = (CItems*)obj;
	}break;
	case OBJECT_TYPE_WEAPON:
	{
		obj = new CWeapon(x, y, player);
		weapon = (CWeapon*)obj;
	}
	case OBJECT_TYPE_BRICK:
	{
		//int width = atoi(tokens[5].c_str());
		//int height = atoi(tokens[6].c_str());
		obj = new CBrick(x, y);
		obj->id_brick_items = atoi(tokens[4].c_str());
		//DebugOut(L"GACH NAM:%d \n", obj->idgachnam);
	}
	break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}
int CPlayScene::RandomItems()
{
	CMario* a = new CMario();
	a = player;
	//int random = rand()%2;
	if (a->level == MARIO_LEVEL_SMALL)
		return CUC_NAM;
	else if (a->level == MARIO_LEVEL_BIG)
		return CHIEC_LA;
	else if (a->level == MARIO_RACCON)
		return FIRE_FLOWER;
	//DebugOut(L"radomitem %d \n", RandomItems());
}
void CPlayScene::Load()
{
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
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	//CBrick* gachthuong = new CBrick();
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
		LPGAMEOBJECT a = objects[i];

		if (dynamic_cast<CBrick*>(a) && a->id_brick_items == 2)
		{
			CBrick* gach = dynamic_cast<CBrick*>(a);
			if (gach->bottom_coll == 1 && gach->created_item == 0 && gach->bouncing == 1)
			{
				gach->created_item = 1;
				listitems.push_back(taonam(gach->x, gach->y));
			}
		}

	}

	/*for (int i = 0; i < objects.size(); i++)
	{
		LPGAMEOBJECT a = objects[i];
		if (dynamic_cast<CBrick*>(a) && a->id_brick_items == 2)
		{
			CBrick* gach = dynamic_cast<CBrick*>(a);
			if (gach->bottom_coll == 1 && gach->created_item == 0 && gach->bouncing ==1)
			{
				gach->created_item = 1;
				listitems.push_back(taonam(gach->x, gach->y));
			}
		}
	}*/
	if (player->use_Weapon && !player->isdone)
	{
		if (player->nx > 0)
			listweapon.push_back(tao_weapon(player->x + 10, player->y + 6, player));
		else
			listweapon.push_back(tao_weapon(player->x - 6, player->y + 6, player));
		player->isdone = true;
	}
	player->vachamvoiitems(&listitems);
	for (int i = 0; i < listweapon.size(); i++)
		listweapon[i]->Update(dt, &coObjects);
	for (int i = 0; i < listitems.size(); i++)
		listitems[i]->Update(dt, &coObjects);

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 3;
	cy -= game->GetScreenHeight() / 3;

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
	//DebugOut(L"Gia tri cua listweapon %d \n", listweapon.size());
	//DebugOut(L"Gia tri cua listitems %d \n", listitems.size());
}

void CPlayScene::Render()
{
	//map->Draw();
	for (int i = 0; i < listweapon.size(); i++)
		listweapon[i]->Render();

	for (int i = 0; i < listitems.size(); i++)
		listitems[i]->Render();

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();


}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	//DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (mario->is_grounded)
		{
			mario->isJumping = false;

		}
		mario->vy = mario->vy + MARIO_GRAVITY * 15 * mario->dt;
		//DebugOut(L"vy = %f \n", mario->vy);
		break;
	case DIK_A:
		mario->isRunning = false;
		if (mario->level == MARIO_FIRE)
		{

			mario->use_Weapon = false;
			mario->isdone = false;
		}
		break;
		//case DIK_Q:
		//	mario->use_Weapon = false;
		//	mario->isdone = false;
		//	break;
	case DIK_RIGHT:
	case DIK_LEFT:
		if (mario->isFlying)
			mario->isFlying = false;

	}
}
void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (mario->level == MARIO_RACCON /*&& mario->isRunning*/)
		{
			/*if (mario->vx > 0.2)
				mario->SetState(MARIO_RACCON_ANI_FLYING_RIGHT);
			else if (mario->vx < -0.2)
				mario->SetState(MARIO_RACCON_ANI_FLYING_LEFT);
			mario->isFlying = true;*/
			if (!mario->isFlying)
			{
				if (mario->isRunning)
				{
					if (mario->vx > 0.2)
						mario->SetState(MARIO_RACCON_ANI_FLYING_RIGHT);
					else if (mario->vx < -0.2)
						mario->SetState(MARIO_RACCON_ANI_FLYING_LEFT);
					mario->isFlying = true;
				}

			}
			else
			{
				if (mario->vx > 0.2)
					mario->SetState(MARIO_RACCON_ANI_FLYING_RIGHT);
				else if (mario->vx < -0.2)
					mario->SetState(MARIO_RACCON_ANI_FLYING_LEFT);
			}
		}
		if (/*mario->isFalling &&*/ mario->level == MARIO_RACCON && mario->isJumping && !mario->isFlying)
		{
			if (!mario->isSitting)
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_RACCON_ANI_FALLING_ROCK_TAIL_RIGHT);
				else
					mario->SetState(MARIO_RACCON_ANI_FALLING_ROCK_TAIL_LEFT);
				//mario->vy = (-0.0006 * 1.2 *mario-> dt);
			}
		}

		if (mario->isJumping)
			return;
		mario->is_grounded = false;
		mario->isJumping = true;
		if (!mario->isFlying) {
			if (mario->level == MARIO_LEVEL_BIG)
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_ANI_BIG_JUMP_RIGHT);
				else
					mario->SetState(MARIO_ANI_BIG_JUMP_LEFT);
			}
			else if (mario->level == MARIO_LEVEL_SMALL)
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_ANI_SMALL_JUMP_RIGHT);
				else
					mario->SetState(MARIO_ANI_SMALL_JUMP_LEFT);
			}
			else if (mario->level == MARIO_RACCON)
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_RACCON_ANI_JUMP_RIGHT);
				else
					mario->SetState(MARIO_RACCON_ANI_JUMP_LEFT);
			}
			else if (mario->level == MARIO_FIRE)
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_FIRE_ANI_JUMP_RIGHT);
				else
					mario->SetState(MARIO_FIRE_ANI_JUMP_LEFT);
			}
			mario->vy = -MARIO_JUMP_SPEED_Y;

		}
		//DebugOut(L"gia tri is high jump %d \n", mario->isHigh_Jumping);
		break;
	case DIK_Z:
		mario->Reset();
		break;
	case DIK_S:
		if (mario->level == MARIO_RACCON)
		{
			if (mario->nx > 0)
				mario->SetState(MARIO_RACCON_ANI_FIGHT_IDLE_RIGHT);
			else
				mario->SetState(MARIO_RACCON_ANI_FIGHT_IDLE_LEFT);
		}
		break;
	case DIK_A:
		if (mario->level == MARIO_FIRE)
		{
			DebugOut(L"A key down");
			mario->use_Weapon = true;
			if (mario->isJumping)
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_FIRE_ANI_FIGHT_WHILE_JUMPING_RIGHT);
				else
					mario->SetState(MARIO_FIRE_ANI_FIGHT_WHILE_JUMPING_LEFT);
			}
			else
			{
				if (mario->nx > 0)
					mario->SetState(MARIO_FIRE_ANI_FIGHT_RIGHT);
				else
					mario->SetState(MARIO_FIRE_ANI_FIGHT_LEFT);
			}
		}

	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 

	if (mario->GetState() == MARIO_STATE_DIE) return;
	if (mario->isWaitingForAni)
		return;
	if (mario->isFlying)
		return;
	if (game->IsKeyDown(DIK_A))
	{
		if (mario->level == MARIO_LEVEL_BIG)
		{
			if (mario->nx > 0)
				mario->SetState(MARIO_ANI_BIG_RUNNING_RIGHT);
			else
				mario->SetState(MARIO_ANI_BIG_RUNNING_LEFT);
		}
		else if (mario->level == MARIO_LEVEL_SMALL)
		{
			if (mario->nx > 0)
				mario->SetState(MARIO_ANI_SMALL_RUNNING_RIGHT);
			else
				mario->SetState(MARIO_ANI_SMALL_RUNNING_LEFT);
		}
		else if (mario->level == MARIO_RACCON)
		{
			if (mario->nx > 0)
				mario->SetState(MARIO_RACCON_ANI_RUNNING_RIGHT);
			else
				mario->SetState(MARIO_RACCON_ANI_RUNNING_LEFT);
		}
		else if (mario->level == MARIO_FIRE)
		{
			if (mario->nx > 0)
				mario->SetState(MARIO_FIRE_ANI_RUNNING_RIGHT);
			else
				mario->SetState(MARIO_FIRE_ANI_RUNNING_LEFT);
		}
	}
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (mario->level == MARIO_LEVEL_BIG)
			mario->SetState(MARIO_ANI_BIG_WALKING_RIGHT);
		else if (mario->level == MARIO_LEVEL_SMALL)
			mario->SetState(MARIO_ANI_SMALL_WALKING_RIGHT);
		else if (mario->level == MARIO_RACCON)
			mario->SetState(MARIO_RACCON_ANI_WALK_RIGHT);
		else if (mario->level == MARIO_FIRE)
			mario->SetState(MARIO_FIRE_ANI_WALK_RIGHT);
		mario->lastnx = 1;
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (mario->level == MARIO_LEVEL_BIG)
			mario->SetState(MARIO_ANI_BIG_WALKING_LEFT);
		else if (mario->level == MARIO_LEVEL_SMALL)
			mario->SetState(MARIO_ANI_SMALL_WALKING_LEFT);
		else if (mario->level == MARIO_RACCON)
			mario->SetState(MARIO_RACCON_ANI_WALK_LEFT);
		else if (mario->level == MARIO_FIRE)
			mario->SetState(MARIO_FIRE_ANI_WALK_LEFT);
		mario->lastnx = -1;
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		if (mario->level == MARIO_LEVEL_BIG)
		{
			if (mario->nx > 0)
				mario->SetState(MARIO_ANI_BIG_SITTING_RIGHT);
			else
				mario->SetState(MARIO_ANI_BIG_SITTING_LEFT);
		}
		else if (mario->level == MARIO_RACCON)
		{
			if (mario->nx > 0)
				mario->SetState(MARIO_RACCON_ANI_SITTING_RIGHT);
			else
				mario->SetState(MARIO_RACCON_ANI_SITTING_LEFT);
		}
		else if (mario->level == MARIO_FIRE)
		{
			if (mario->nx > 0)
				mario->SetState(MARIO_FIRE_ANI_SITTING_RIGHT);
			else
				mario->SetState(MARIO_FIRE_ANI_SITTING_LEFT);
		}
	}
	else
	{
		if (mario->isJumping)
		{
			if (mario->vy < 0)
			{
				if (mario->level == MARIO_LEVEL_BIG)
				{
					if (mario->nx > 0)
						mario->state = MARIO_ANI_BIG_JUMP_RIGHT;
					else
						mario->state = MARIO_ANI_BIG_JUMP_LEFT;
				}
				else if (mario->level == MARIO_RACCON)
				{
					if (mario->nx > 0)
						mario->state = MARIO_RACCON_ANI_JUMP_RIGHT;
					else
						mario->state = MARIO_RACCON_ANI_JUMP_LEFT;
				}
				else if (mario->level == MARIO_FIRE)
				{
					if (mario->nx > 0)
						mario->state = MARIO_FIRE_ANI_JUMP_RIGHT;
					else
						mario->state = MARIO_FIRE_ANI_JUMP_LEFT;
				}
			}
			else
			{
				if (mario->level == MARIO_LEVEL_BIG)
				{
					if (mario->nx > 0)
						mario->state = MARIO_ANI_BIG_FALLING_RIGHT;
					else
						mario->state = MARIO_ANI_BIG_FALLING_LEFT;
				}
				else if (mario->level == MARIO_RACCON)
				{
					if (mario->nx > 0)
						mario->state = MARIO_RACCON_ANI_FALLING_RIGHT;
					else
						mario->state = MARIO_RACCON_ANI_FALLING_LEFT;
				}
				else if (mario->level == MARIO_FIRE)
				{
					if (mario->nx > 0)
						mario->state = MARIO_FIRE_ANI_FALLING_RIGHT;
					else
						mario->state = MARIO_FIRE_ANI_FALLING_LEFT;
				}
			}
			return;
		}
		mario->SetState(MARIO_STATE_IDLE);
		mario->isSitting = false;
	}
	//DebugOut(L"bbbbbbbbbbbb %d\n", mario->isWaitingForAni);
}