#pragma once
#include "GameObject.h"
#include "Game.h"

#define CELL_WIDTH 260
#define CELL_HEIGHT 260

#define MAP_WIDTH 3328
#define MAP_HEIGHT  1000


class CGrid
{
	//int mapWidth, mapHeight;
	//int cellWidth, cellHeight;
	vector<vector<vector<LPGAMEOBJECT>>> listCells;
public:
	CGrid(int cellWidth = CELL_WIDTH, int cellHeight = CELL_HEIGHT);
	~CGrid();
	void Resize();
	void ClearGrid(int numsRow, int numsCol);
	void ResetGrid(vector<LPGAMEOBJECT> list);
	//void PushGrid(vector<LPGAMEOBJECT> list);
	void GetGrid(vector<LPGAMEOBJECT>& list);
	//void PushGridStart(LPGAMEOBJECT obj, int row, int col);
	void MakeObjOutOfCam(vector<LPGAMEOBJECT>& list);
};
