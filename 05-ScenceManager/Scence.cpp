#include "Scence.h"

CScene::CScene(int id, LPCWSTR filePath)
{
	//backUpLevel = 1;
	this->id = id;
	this->sceneFilePath = filePath;
}