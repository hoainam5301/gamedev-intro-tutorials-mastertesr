#pragma once
#include"Sprites.h"
#include"Textures.h"
#include"Utils.h"
#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include"Game.h"
#define SCENE_1 1
#define SCENE_2 2
#define SCENE_1_ROW 10
#define SCENE_1_COL 48
#define SCENE_1_ROW_READ 4
#define SCENE_1_COL_READ 17
#define SCENE_2_ROW 12
#define SCENE_2_COL 17
#define SCENE_2_ROW_READ 4
#define SCENE_2_COL_READ 7

#define TILE_WIDTH 32
#define TILE_HEIGHT 32

#define MAX_SCENE_LINE 1024
#define map2 4000
#define map3 6000
#define map6 12000
#define map4 8000

#define max_row 100
#define max_col 100

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
	int getwidthmap();
	int getid() { return id; }
	void Load();
	void LoadMap(int id, LPCWSTR file_pic, LPCWSTR filePath_data, int num_row, int num_col, int num_row_read, int num_col_read, int tile_width, int tile_height, int r, int g, int b);
	void Draw();
};