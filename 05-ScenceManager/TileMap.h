#include"Textures.h"
#include <iostream>
#include <fstream>
#include "Game.h"
class TileMap
{
	int collum;
	int row;
	int TiledCollum, TileRow;
	int TiledID[200][200];
	CTextures* textures = new CTextures();
public:
	TileMap();
	void ReadMap();
	void DrawMap();
	~TileMap();
};