#pragma once
#include"Sprites.h"
#include"Textures.h"
#include"Utils.h"
#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include"Game.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16

#define max_row 200
#define max_col 200

class TileMap
{
	CSprites* sprites;
	LPCWSTR mapFilePath;

	int num_row, num_col;
	int tile_width, tile_height;
	int id;


	int tilemap[max_row][max_col];

public:
	TileMap();
	~TileMap();
	//int getwidthmap();
	int getid() { return id; }
	void Load();
	void LoadMap(int id, LPCWSTR file_pic, LPCWSTR filePath_data, int num_row, int num_col, int num_row_read, int num_col_read, int tile_width, int tile_height, int r, int g, int b);
	void Draw();
};