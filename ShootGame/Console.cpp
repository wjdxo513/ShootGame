#include <windows.h>
#include <stdio.h>
#include <locale.h>
#include "Console.h"
#include <conio.h>

HANDLE  hConsole;
//-------------------------------------------------------------
// �ܼ� ��� ���� �غ� �۾�.
//
//-------------------------------------------------------------
void cs_Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;

	//-------------------------------------------------------------
	// ȭ���� Ŀ���� �Ⱥ��̰Բ� �����Ѵ�.
	//-------------------------------------------------------------
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize   = 1;			// Ŀ�� ũ��.
											// �̻��ϰԵ� 0 �̸� ���´�. 1���ϸ� �ȳ��´�.

	//-------------------------------------------------------------
	// �ܼ�ȭ�� (���ٴٵ� �ƿ�ǲ) �ڵ��� ���Ѵ�.
	//-------------------------------------------------------------
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);

	//�ܼ� ũ�� ����
	char setText[100];
	sprintf_s(setText, "mode con cols=%d lines=%d", SCREEN_WIDTH, SCREEN_HEIGHT);
	system(setText);

}

//-------------------------------------------------------------
// �ܼ� ȭ���� Ŀ���� X, Y ��ǥ�� �̵���Ų��.
//
//-------------------------------------------------------------
void cs_MoveCursor(int iPosX, int iPosY)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;
	//-------------------------------------------------------------
	// ���ϴ� ��ġ�� Ŀ���� �̵���Ų��.
	//-------------------------------------------------------------
	SetConsoleCursorPosition(hConsole, stCoord);
}

//-------------------------------------------------------------
// �ܼ� ȭ���� �ʱ�ȭ �Ѵ�.
//
//-------------------------------------------------------------
void cs_ClearScreen(void){

	int iCountX, iCountY;
	DWORD dw;

	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 100*100, { 0, 0 }, &dw);


	//-------------------------------------------------------------
	// ȭ�� ũ�⸸ŭ ����, ���� ���� for ���� ����Ͽ�
	// ������ ��ǥ���� printf(" ");  ������ ���� ��� ���ش�.
	//-------------------------------------------------------------
	for ( iCountY = 0 ; iCountY < SCREEN_HEIGHT; iCountY++ )
	{
		for ( iCountX = 0; iCountX < SCREEN_WIDTH; iCountX++ )
		{
			cs_MoveCursor(iCountX, iCountY);
			printf(" ");
		}
	}
}

//--------------------------------------------------------------------
// ������ ������ ȭ������ ����ִ� �Լ�.
//--------------------------------------------------------------------
void Buffer_Print(int i, int j, char c) {

	switch (c)
	{
	case 48:
		setColor(YELLOW);
		Sprite_Draw(i, j, L'@');
		break;
	case 49:
		setColor(GREEN);
		Sprite_Draw(i, j, L'+');
		break;
	case 50:
		setColor(BLUE);
		Sprite_Draw(i, j, L'A');
		break;
	case 51:
		setColor(RED);
		Sprite_Draw(i, j, L'0');
		break;
	default:
		Sprite_Draw(i, j, L' ');
	}
}

//--------------------------------------------------------------------
// ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���.
//
// �Է� ���� X,Y ��ǥ�� �ƽ�Ű�ڵ� �ϳ��� ����Ѵ�. (���ۿ� �׸�)
//--------------------------------------------------------------------
void Sprite_Draw(int iX, int iY, wchar_t chSprite){
	cs_MoveCursor(iX, iY);
	setlocale(LC_ALL, "");
	wprintf(L"%lc", chSprite);
}

//--------------------------------------------------------------------
// �ܼ� �ؽ�Ʈ ���� �������ִ� �Լ�
//--------------------------------------------------------------------
void setColor(unsigned short text) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

//--------------------------------------------------------------------
// ����â ���� Function
//--------------------------------------------------------------------
void game_Function(int x, int y, char str[], int color) {
	setColor(color);
	cs_MoveCursor(x, y);
	printf("%s", str);
}

//--------------------------------------------------------------------
// Game Over �� ������ ����â
//--------------------------------------------------------------------
void game_Over() {
	system("cls");
	char str[] = "GAME OVER !!";
	game_Function(SCREEN_WIDTH / 2 - 6, SCREEN_HEIGHT / 2, str, 12);
}

//--------------------------------------------------------------------
// Game Stage Clear �� ������ ����â
//--------------------------------------------------------------------
void game_Clear() {
	system("cls");
	char str1[] = "Stage Clear !!";
	char str2[] = "Next Stage !!";
	game_Function(SCREEN_WIDTH / 2 - 6, SCREEN_HEIGHT / 2, str1, 14);
	Sleep(1000);

	game_Function(SCREEN_WIDTH / 2 - 6, SCREEN_HEIGHT / 2, str2, 14);
	Sleep(500);
	system("cls");
}

//--------------------------------------------------------------------
// Game Start �� ������ ����â
//--------------------------------------------------------------------
void game_Start() {
	system("cls");
	char str1[] = "C���� ����� ���� ����";
	char str2[] = "~ Press Any Key ~";
	char str3[] = "- 2021.02.04 WJT -";

	game_Function(SCREEN_WIDTH / 2 - 15, SCREEN_HEIGHT / 2 - 2, str1, 15);
	game_Function(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 2, str2, 15);
	game_Function(SCREEN_WIDTH / 2 + 22, SCREEN_HEIGHT - 2, str3, 15);

	char c = _getch(); //�Է°� input
	system("cls");
}

//--------------------------------------------------------------------
// Game End �� ������ ����â
//--------------------------------------------------------------------
void game_End() {
	system("cls");
	char str1[] = "��� �������� Clear !!";
	char str2[] = "�����մϴ� !!";
	game_Function(SCREEN_WIDTH / 2 -10, SCREEN_HEIGHT / 2, str1, 14);
	game_Function(SCREEN_WIDTH / 2 - 6, SCREEN_HEIGHT / 2 + 4, str2, 14);
	Sleep(2000);
}