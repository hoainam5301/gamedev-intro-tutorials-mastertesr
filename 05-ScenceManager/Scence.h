#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"

class CScene
{
protected:
	CKeyEventHandler * key_handler;
	int id;
	LPCWSTR sceneFilePath;
	//int backUpLevel;
public: 
	bool isLoad = false;
	CScene(int id, LPCWSTR filePath);
	
	CKeyEventHandler * GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(ULONGLONG dt) = 0;
	virtual void Render() = 0; 
	//void SetBackUpMario(int level) { backUpLevel = level; }
};
typedef CScene * LPSCENE;


class CScenceKeyHandler : public CKeyEventHandler
{
protected: 
	CScene * scence; 

public:
	virtual void KeyState(BYTE *states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScene *s) :CKeyEventHandler() { scence = s; }
};