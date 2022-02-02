#pragma once

//////////////////////////////////////////////////////////////
// Windows 의 콘솔 화면에서 커서 제어.
//
//////////////////////////////////////////////////////////////
#ifndef __CONSOLE__
#define __CONSOLE__

#define SCREEN_WIDTH		81		// 콘솔 가로 80칸 + NULL
#define SCREEN_HEIGHT		29		// 콘솔 세로 24칸

//색상
enum {
	BLACK,
	DARK_BLUE,
	DARK_GREEN,
	DARK_SKYBLUE,
	DARK_RED,
	DARK_VOILET,
	DAKR_YELLOW,
	GRAY,
	DARK_GRAY,
	BLUE,
	GREEN,
	SKYBLUE,
	RED,
	VIOLET,
	YELLOW,
	WHITE,
};
//-------------------------------------------------------------
// 콘솔 제어를 위한 준비 작업.
//
//-------------------------------------------------------------
void cs_Initial(void);


//-------------------------------------------------------------
// 콘솔 화면의 커서를 X, Y 좌표로 이동시킨다.
//
//-------------------------------------------------------------
void cs_MoveCursor(int iPosX, int iPosY);

//-------------------------------------------------------------
// 콘솔 화면을 초기화 한다.
//
//-------------------------------------------------------------
void cs_ClearScreen(void);

//신규로 추가한 함수들
void Sprite_Draw(int iX, int iY, wchar_t chSprite);
void Buffer_Print(int i, int j, char c);
void setColor(unsigned short text);
void game_Over();
void game_Start();
void game_Clear();
void game_End();
#endif
