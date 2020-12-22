#include "Grid.h"
#include "Mario.h"

CGrid::CGrid(int cellWidth, int cellHeight)
{

}

void CGrid::Resize()
{
	int totalCol = MAP_WIDTH / CELL_WIDTH;
	int totalRow = MAP_HEIGHT / CELL_HEIGHT;

	listCells.resize(totalRow + 1);
	for (int i = 0; i < totalRow + 1; i++)
		listCells[i].resize(totalCol + 1);

	ClearGrid(totalRow, totalCol);
}

void CGrid::ClearGrid(int numRow, int numCol)
{
	for (int i = 0; i < numRow; i++)
		for (int j = 0; j < numCol; j++)
			listCells[i][j].clear();
}

void CGrid::ResetGrid(vector<LPGAMEOBJECT> list)
{
	ClearGrid((int)MAP_HEIGHT / CELL_HEIGHT, (int)MAP_WIDTH / CELL_WIDTH);
	
	//float l, t, r, b;
	//list[0] = dynamic_cast<CMario*>(list[0]);
	//list[0]->GetBoundingBox(l, t, r, b);
	
	for (int i = 0; i < list.size(); i++)
	{
		float l, t, r, b;	
		list[i]->GetBoundingBox(l, t, r, b);
		int Top = int(t / CELL_HEIGHT);
		int Left = int(l / CELL_WIDTH);
		int Right = ceil(r / CELL_WIDTH);
		int Bottom = ceil(b / CELL_HEIGHT);
		if (!list[i]->isdone)
		{
			for (int row = Top; row < Bottom; row++)
				for (int col = Left; col < Right; col++)
					listCells[row][col].push_back(list[i]);
		}
	}
}

void CGrid::MakeObjOutOfCam(vector<LPGAMEOBJECT> &list)
{
	for (int i = 0; i < list.size(); i++)
	{
		list[i]->checkOnCam = false;
	}
}

void CGrid::GetGrid(vector<LPGAMEOBJECT>& list)
{
	int firstCol = (int)(CGame::GetInstance()->GetCamPosX() / CELL_WIDTH);
	int lastCol = (int)((CGame::GetInstance()->GetCamPosX() + SCREEN_WIDTH) / CELL_WIDTH);
	int totalRow = MAP_HEIGHT / CELL_HEIGHT;
	
	for (int i = 0; i < totalRow; i++)
	{
		for (int j = firstCol; j < lastCol; j++)
		{
			for (int k = 0; k < listCells[i][j].size(); k++)
			{
				if (!listCells[i][j][k]->checkOnCam && !listCells[i][j][k]->isdone)
				{
					listCells[i][j][k]->checkOnCam = true;
					list.push_back(listCells[i][j][k]);
				}
			}
		}
	}
}

