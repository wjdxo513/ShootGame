#include <windows.h>
#include <stdio.h>
#include <locale.h>
#include "Console.h"
#include <conio.h>

HANDLE  hConsole;
//-------------------------------------------------------------
// 콘솔 제어를 위한 준비 작업.
//
//-------------------------------------------------------------
void cs_Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;

	//-------------------------------------------------------------
	// 화면의 커서를 안보이게끔 설정한다.
	//-------------------------------------------------------------
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize   = 1;			// 커서 크기.
											// 이상하게도 0 이면 나온다. 1로하면 안나온다.

	//-------------------------------------------------------------
	// 콘솔화면 (스텐다드 아웃풋) 핸들을 구한다.
	//-------------------------------------------------------------
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);

	//콘솔 크기 조절
	char setText[100];
	sprintf_s(setText, "mode con cols=%d lines=%d", SCREEN_WIDTH, SCREEN_HEIGHT);
	system(setText);

}

//-------------------------------------------------------------
// 콘솔 화면의 커서를 X, Y 좌표로 이동시킨다.
//
//-------------------------------------------------------------
void cs_MoveCursor(int iPosX, int iPosY)
{
	COORD stCoord;
	stCoord.X = iPosX;
	stCoord.Y = iPosY;
	//-------------------------------------------------------------
	// 원하는 위치로 커서를 이동시킨다.
	//-------------------------------------------------------------
	SetConsoleCursorPosition(hConsole, stCoord);
}

//-------------------------------------------------------------
// 콘솔 화면을 초기화 한다.
//
//-------------------------------------------------------------
void cs_ClearScreen(void){

	int iCountX, iCountY;
	DWORD dw;

	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 100*100, { 0, 0 }, &dw);


	//-------------------------------------------------------------
	// 화면 크기만큼 세로, 가로 이중 for 문을 사용하여
	// 각각의 좌표마다 printf(" ");  공백을 전부 출력 해준다.
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
// 버퍼의 내용을 화면으로 찍어주는 함수.
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
// 버퍼의 특정 위치에 원하는 문자를 출력.
//
// 입력 받은 X,Y 좌표에 아스키코드 하나를 출력한다. (버퍼에 그림)
//--------------------------------------------------------------------
void Sprite_Draw(int iX, int iY, wchar_t chSprite){
	cs_MoveCursor(iX, iY);
	setlocale(LC_ALL, "");
	wprintf(L"%lc", chSprite);
}

//--------------------------------------------------------------------
// 콘솔 텍스트 색상 변경해주는 함수
//--------------------------------------------------------------------
void setColor(unsigned short text) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

//--------------------------------------------------------------------
// 상태창 공통 Function
//--------------------------------------------------------------------
void game_Function(int x, int y, char str[], int color) {
	setColor(color);
	cs_MoveCursor(x, y);
	printf("%s", str);
}

//--------------------------------------------------------------------
// Game Over 시 나오는 상태창
//--------------------------------------------------------------------
void game_Over() {
	system("cls");
	char str[] = "GAME OVER !!";
	game_Function(SCREEN_WIDTH / 2 - 6, SCREEN_HEIGHT / 2, str, 12);
}

//--------------------------------------------------------------------
// Game Stage Clear 시 나오는 상태창
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
// Game Start 시 나오는 상태창
//--------------------------------------------------------------------
void game_Start() {
	system("cls");
	char str1[] = "C언어로 만드는 슈팅 게임";
	char str2[] = "~ Press Any Key ~";
	char str3[] = "- 2021.02.04 WJT -";

	game_Function(SCREEN_WIDTH / 2 - 15, SCREEN_HEIGHT / 2 - 2, str1, 15);
	game_Function(SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 + 2, str2, 15);
	game_Function(SCREEN_WIDTH / 2 + 22, SCREEN_HEIGHT - 2, str3, 15);

	char c = _getch(); //입력값 input
	system("cls");
}

//--------------------------------------------------------------------
// Game End 시 나오는 상태창
//--------------------------------------------------------------------
void game_End() {
	system("cls");
	char str1[] = "모든 스테이지 Clear !!";
	char str2[] = "축하합니다 !!";
	game_Function(SCREEN_WIDTH / 2 -10, SCREEN_HEIGHT / 2, str1, 14);
	game_Function(SCREEN_WIDTH / 2 - 6, SCREEN_HEIGHT / 2 + 4, str2, 14);
	Sleep(2000);
}