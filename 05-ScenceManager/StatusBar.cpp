#include "StatusBar.h"


StatusBar::StatusBar(CMario* mario)
{
	Mario = mario;
	time = 100;
}

void StatusBar::DrawNumber(float x, float y, string stringnumb,int maxsize)
{
	stringnumb = string (maxsize - stringnumb.size(), '0').append(stringnumb);
	for (int i = 0; i < stringnumb.size(); i++)
	{
		if (stringnumb[i] == ' ')
			continue;
		int numb;
		for (int j = 0; j < 10; j++)
		{
			numb = 48 + j;     // 48 la gia tri 0 trong bang ma ascii
			if (stringnumb[i] == numb)
			{
				number = CSprites::GetInstance()->Get(2010 + j);
				break;
			}
		}
		number->Draw(x + (i * 8)+X, y);
	}
}
void StatusBar::DrawBoard()
{
	CSprites::GetInstance()->Get(2000)->Draw(posX+3,posY+233);
}
void StatusBar::DrawPMeter()
{
	if(Mario->isFlying)
		CAnimations::GetInstance()->Get(773)->Render(posX + 52 + X, posY + Y + 7);
	else if (Mario->isRunning)
	{
		float marioSpeed = abs(Mario->vx);

		if (marioSpeed < SPEED_X_TIRER_1)
			CSprites::GetInstance()->Get(2020)->Draw(posX + 52 + X, posY + Y + 7);
		else if (marioSpeed < SPEED_X_TIRER_2)
			CSprites::GetInstance()->Get(2021)->Draw(posX + 52 + X, posY + Y + 7);
		else if (marioSpeed < SPEED_X_TIRER_3)
			CSprites::GetInstance()->Get(2022)->Draw(posX + 52 + X, posY + Y + 7);
		else if (marioSpeed < SPEED_X_TIRER_4)
			CSprites::GetInstance()->Get(2023)->Draw(posX + 52 + X, posY + Y + 7);
		else if (marioSpeed < SPEED_X_TIRER_5)
			CSprites::GetInstance()->Get(2024)->Draw(posX + 52 + X, posY + Y + 7);
		else if (marioSpeed < SPEED_X_TIRER_6)
			CSprites::GetInstance()->Get(2025)->Draw(posX + 52 + X, posY + Y + 7);
		else if (marioSpeed < SPEED_X_TIRER_7)
			CSprites::GetInstance()->Get(2026)->Draw(posX + 52 + X, posY + Y + 7);
		else if (marioSpeed <= SPEED_X_TIRER_8)
			CAnimations::GetInstance()->Get(773)->Render(posX + 52 + X, posY + Y + 7);
	}
	
}
void StatusBar::DrawAllNumber()
{
	
	DrawNumber(posX + 37, posY + Y + 7, to_string(word),1);// word 1,2,3
	DrawNumber(posX + 29, posY + Y+15, to_string(life),1);//so mang con lai
	DrawNumber(posX + 52, posY + Y + 15, to_string(score),7);//diem so
	DrawNumber(posX + 132, posY + Y + 7, to_string(money),2);//money
	DrawNumber(posX + 124, posY + Y + 15, to_string(time),3);//thoi gian
}

void StatusBar::Update(ULONGLONG dt, float camX, float camY)
{
	posX = camX;
	posY = camY;
	score = 1000;
	money = 99;
	word = 1;
	life = 4;
	second++;
	if (second == 60)
	{
		time--;
		second = 1;
	}	
}

void StatusBar::Render()
{
	DrawBoard();
	DrawAllNumber();
	DrawPMeter();
}
