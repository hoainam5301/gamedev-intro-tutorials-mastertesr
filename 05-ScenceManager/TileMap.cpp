
#include "TileMap.h"

TileMap::TileMap()
{
	sprites = CSprites::GetInstance();
	this->tile_width = TILE_WIDTH;
	this->tile_height = TILE_HEIGHT;

}

void TileMap::Load()
{


	DebugOut(L"[INFO] Start loading map resources from : %s \n", mapFilePath);

	ifstream f;
	f.open(mapFilePath);


	if (f.fail())
	{
		DebugOut(L"[ERROR] TileMap::Load_MapData failed: ID=%d", id);
		f.close();
		return;
	}


	for (UINT i = 0; i < 27; i++)
	{
		for (UINT j = 0; j < 176; j++)
		{
			f >> tilemap[i][j];
		}
	}
	f.close();


	DebugOut(L"[INFO] Done loading map resources %s\n", mapFilePath);
}

void TileMap::LoadMap(int id, LPCWSTR file_pic, LPCWSTR filePath_data, int num_row, int num_col, int num_row_read, int num_col_read, int tile_width, int tile_height, int r, int g, int b)
{
	CTextures* texture = CTextures::GetInstance();
	//
	this->id = id;
	DebugOut(L"[INFO] IDMAP\n", id);
	//this->mappic = file_pic;
	this->mapFilePath = filePath_data;
	this->num_row = 27;
	this->num_col = 176;
	this->tile_width = 16;
	this->tile_height = 16;

	texture->Add(id, file_pic, D3DCOLOR_XRGB(r, g, b));

	LPDIRECT3DTEXTURE9 texTileMap = texture->Get(id);


	int idsprite = 1;
	for (UINT i = 0; i < num_row_read; i++)
	{
		for (UINT j = 0; j < num_col_read; j++)
		{
			int id_SPRITE = id + idsprite;
			sprites->Add(id_SPRITE, 16 * j, 16 * i, 16 * (j + 1), 16 * (i + 1), texTileMap);
			idsprite++;
		}
	}

	Load();
}

void TileMap::Draw()
{

	int firstcol = (int)CGame::GetInstance()->GetCamPosX() / 16;
	int lastcol = firstcol + (SCREEN_WIDTH / 16);

	for (UINT i = 0; i < 27; i++)
	{
		for (UINT j = firstcol; j <= lastcol; j++)
		{
			float x = 16 * (j - firstcol) + CGame::GetInstance()->GetCamPosX() - (int)(CGame::GetInstance()->GetCamPosX()) % 16;
			float y = 16 * i + 80;


			sprites->Get(tilemap[i][j] + id)->Draw(-1, x, y);

		}
	}


}

TileMap::~TileMap()
{
}
