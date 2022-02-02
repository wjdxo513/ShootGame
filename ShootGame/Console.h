#pragma once

//////////////////////////////////////////////////////////////
// Windows �� �ܼ� ȭ�鿡�� Ŀ�� ����.
//
//////////////////////////////////////////////////////////////
#ifndef __CONSOLE__
#define __CONSOLE__

#define SCREEN_WIDTH		81		// �ܼ� ���� 80ĭ + NULL
#define SCREEN_HEIGHT		29		// �ܼ� ���� 24ĭ

//����
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
// �ܼ� ��� ���� �غ� �۾�.
//
//-------------------------------------------------------------
void cs_Initial(void);


//-------------------------------------------------------------
// �ܼ� ȭ���� Ŀ���� X, Y ��ǥ�� �̵���Ų��.
//
//-------------------------------------------------------------
void cs_MoveCursor(int iPosX, int iPosY);

//-------------------------------------------------------------
// �ܼ� ȭ���� �ʱ�ȭ �Ѵ�.
//
//-------------------------------------------------------------
void cs_ClearScreen(void);

//�űԷ� �߰��� �Լ���
void Sprite_Draw(int iX, int iY, wchar_t chSprite);
void Buffer_Print(int i, int j, char c);
void setColor(unsigned short text);
void game_Over();
void game_Start();
void game_Clear();
void game_End();
#endif
