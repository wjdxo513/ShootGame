#include <stdio.h>
#include <memory.h>
#include <Windows.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include "Console.h"

//전역 변수
char szScreenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
char frontBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

const int MAX = 100;
int iX, iY; //캐릭터 좌표
int b_index; //총알 배열 index
int eb_index; //적 총알 배열 index

char stage_List[MAX][MAX]; // 스테이지 리스트 
int enemy_Num = 0; // 적 숫자
int game_Score = 0; // 게임 점수
int game_Status = 1; // 게임 상태 (0 = 진행중, 1 = GameOver, 2 = Stage Clear 4 = Stage End)
int game_Stage; // 스테이지

struct Point { //좌표
	int X = 0;
	int Y = 0;
	bool Active = false;
};
Point Enemy[MAX]; //적
Point Bullet[MAX]; //총알
Point Enemy_Bullet[MAX]; //적 총알

//함수 Set
void Buffer_Flip(void);
void Buffer_Clear(void);
void Sprite_Draw(int iX, int iY, wchar_t chSprite);
void Buffer_Print(int j, int i, char c);
void Bullet_Shoot();
void Charector_Move();
void Enemy_Create(); //적 생성
void Enemy_Attack(); //적 공격
void Enemy_Move(); //적 움직임
void ShowOption(); //상태창 표시
void init_Stage(); //스테이지 로드
void init_Game(); //게임 로드

//Main 함수 시작
void main(void)
{
	cs_Initial();
	game_Start();
	init_Game();

	while (game_Status == 1) {
		Buffer_Clear();
		init_Stage();

		//스테이지 시작
		while (game_Status == 0)
		{
			// 스크린 버퍼를 지움
			Buffer_Clear();

			//상태창 표시
			ShowOption();

			// 캐릭터 제어
			Charector_Move();

			//총알 발사
			Bullet_Shoot();

			//적 표현
			Enemy_Create();
			Enemy_Move();
			Enemy_Attack();

			// 스크린 버퍼를 화면으로 출력
			Buffer_Flip();

			// 프레임 맞추기용 대기 10 Frame
			Sleep(100);
		}
		if(game_Status==2) game_Over();
		if (game_Status == 4)return;
		if (game_Status == 1) {
			game_Clear();
			game_Stage++;
		}
	}
}

//--------------------------------------------------------------------
// 이전 버퍼와 이후 버퍼를 비교하는 함수
//
// szScreenBuffer와 front_buffer를 비교하여 다르다면 Buffer_Print 함수를 호출한다.
//--------------------------------------------------------------------
void Buffer_Flip(void)
{
	//현재 szScreenBuffer와 front_buffer 비교
	for (int i = 0; i < SCREEN_HEIGHT; ++i) {
		for (int j = 0; j < SCREEN_WIDTH; ++j)
		{
			if (frontBuffer[i][j] != szScreenBuffer[i][j])
			{
				Buffer_Print(j, i, szScreenBuffer[i][j]);
				// 바뀐 부분 화면에 출력
				frontBuffer[i][j] = szScreenBuffer[i][j];
				// 바뀐 부분은 출력 후 front_buffer에 저장
			}
		}
		SetConsoleActiveScreenBuffer(szScreenBuffer[i]);
	}
}


//--------------------------------------------------------------------
// 화면 버퍼를 지워주는 함수
//
// 매 프레임 그림을 그리기 직전에 버퍼를 지워 준다. 
// 안그러면 이전 프레임의 잔상이 남으니까
//--------------------------------------------------------------------
void Buffer_Clear(void)
{
	for (int i = 0; i < SCREEN_HEIGHT - 5; i++) {
		for (int j = 0; j < SCREEN_WIDTH; j++) {
			szScreenBuffer[i][j] = ' ';
		}
	}
}


//--------------------------------------------------------------------
// 캐릭터의 좌표를 이동
//
// 방향키를 입력받아 X,Y좌표를 이동시킨다.
//--------------------------------------------------------------------
void Charector_Move() {

	if (GetAsyncKeyState(VK_LEFT)) { //왼쪽
		iX--;
		if (iX < 0) iX = 0;
	}
	if (GetAsyncKeyState(VK_RIGHT)) { //오른쪽
		iX++;
		if (iX > 78) iX = 78;
	}
	if (GetAsyncKeyState(VK_UP)) { //위
		iY--;
		if (iY < 0) iY = 0;
	}
	if (GetAsyncKeyState(VK_DOWN)) { //아래
		iY++;
		if (iY > 23) iY = 23;
	}if (GetAsyncKeyState(VK_CONTROL)) { //Ctrl 장전
		b_index = 0;
	}
	// 스크린 버퍼에 캐릭터 출력
	szScreenBuffer[iY][iX] = 48;

}

//--------------------------------------------------------------------
// 총알발사와 총알의 좌표를 이동, 총알과 적 충돌
//
// Space확인 후 총알 발사와 좌표를 한칸씩 위로 이동시킨다.
//--------------------------------------------------------------------
void Bullet_Shoot() {
	
	//총알 이동
	for (int i = 0; i < MAX; i++) {
		if (Bullet[i].Active) {
			Bullet[i].Y--;
			szScreenBuffer[Bullet[i].Y][Bullet[i].X] = 49;

			if (Bullet[i].Y < 1) {
				Bullet[i].Active = false;
			}

			//총알 적 충돌
			for (int j = 0; j < MAX; j++) {
				if (Enemy[j].Active && Enemy[j].X == Bullet[i].X && Enemy[j].Y == Bullet[i].Y) {
					Enemy[j].Active = false; //적 상태 비활성화
					Bullet[i].Active = false; //총알 상태 비활성화
					game_Score += 1000; //점수 +1000
					enemy_Num--; //적 숫자 -1
				}
			}
		}
	}

	//총알 발사
	if (GetAsyncKeyState(VK_SPACE)) { //SPACE
		if (b_index > 10)return;
		Bullet[b_index].X = iX;
		Bullet[b_index].Y = iY - 1;
		Bullet[b_index].Active = true;
		b_index++;
	}
}

//--------------------------------------------------------------------
// 적 생성
//--------------------------------------------------------------------
void Enemy_Create() {
	for (int i = 0; i < MAX; i++) {
		if (Enemy[i].Active) {
			szScreenBuffer[Enemy[i].Y][Enemy[i].X] = 50;
		}
	}
}
//--------------------------------------------------------------------
// 적의 움직임 함수 
// 더 이상 움직일곳이 없으면 반대방향으로 움직임
// 그것이 아니라면 무작위로 움직임
//--------------------------------------------------------------------
void Enemy_Move() {

	for (int i = 0; i < MAX; i++) {
		if (Enemy[i].Active == FALSE) {
			continue;
		}
		else {
			//	MOVE
			if (Enemy[i].X <= 0 )Enemy[i].X++;
			else if (Enemy[i].X >= 80)Enemy[i].X--;
			else if (Enemy[i].Y <= 0)Enemy[i].Y++;
			else if (Enemy[i].Y >= 21)Enemy[i].Y--;
			else {
				if (rand() % 4 == 0) {
					Enemy[i].X++;
				}else if (rand() % 4 == 1) {
					Enemy[i].X--;
				}else if (rand() % 4 == 2) {
					Enemy[i].Y++;
				}else if (rand() % 4 == 3) {
					Enemy[i].Y--;
				}
			}
		}
	}
}


//--------------------------------------------------------------------
// 적의 총알 움직임
//
// 적의 총알 발사와 좌표를 한칸씩 아래로 이동시킨다.
//--------------------------------------------------------------------
void Enemy_Attack() {
	
	//적 총알 이동
	for (int i = 0; i < MAX; i++) {
		if (Enemy_Bullet[i].Active) {
			Enemy_Bullet[i].Y++;
			szScreenBuffer[Enemy_Bullet[i].Y][Enemy_Bullet[i].X] = 51;

			if (Enemy_Bullet[i].Y >= 23) {
				Enemy_Bullet[i].Active = false;
			}

			//적 총알과 캐릭터의 충돌 확인
			if (iX == Enemy_Bullet[i].X && iY == Enemy_Bullet[i].Y) {
				game_Status = 2;
				game_Over();
			}
		}
	}

	if (rand() % 10 != 0)return; //발사될 확률
	if (eb_index == MAX-1)eb_index = 0; //총알 초기화
	//적 총알 발사
	for (int i = 0; i < MAX; i++) {
		if (Enemy[i].Active) {
			Enemy_Bullet[eb_index].X = Enemy[i].X;
			Enemy_Bullet[eb_index].Y = Enemy[i].Y + 1;
			Enemy_Bullet[eb_index].Active = true;
			eb_index++;
		}
	}
}

//--------------------------------------------------------------------
// 점수, 총알, 스테이지, 남은 적 수 등 상태 (체크 및 표시)
//--------------------------------------------------------------------
void ShowOption() {
	game_Score++;

	cs_MoveCursor(1, 25);// 커서를 1, 25 위치로
	setColor(WHITE);
	printf("총알 : ");
	for (int i = 0; i <= 10; i++) {
		if (b_index-1 >= i) {
			setColor(WHITE);
		}
		else {
			setColor(VIOLET);
		}
		printf("■");
	}

	setColor(WHITE);
	cs_MoveCursor(61, 25); // 커서를 60, 25 위치로
	printf("이동 : ↑,↓,←,→");
	cs_MoveCursor(61, 26);
	printf("공격 : Space");
	cs_MoveCursor(61, 27);
	printf("장전 : Ctrl");
	cs_MoveCursor(1, 27);
	printf("점수 : %d", game_Score);
	cs_MoveCursor(68, 0); // 커서를 60, 25 위치로
	
	char* str = NULL;
	printf("스테이지: %d", game_Stage + 1);

	if (enemy_Num == 0) {
		game_Status = 1; //적 모두 제거 시 스테이지 Clear
	}
}

//--------------------------------------------------------------------
// 스테이지 로드
//--------------------------------------------------------------------
void init_Stage() {

	if (strcmp(stage_List[game_Stage],"")==0) { //스테이지가 없을 경우 종료
		game_Status = 4;
		game_End();
		return;
	}

	iX = 40, iY = 23; //캐릭터 좌표
	b_index = 0; //총알 배열 index
	eb_index = 0; //적 총알 배열 index
	game_Status = 0; //게임 상태
	enemy_Num = 0; //적 숫자
	
	//모든 스테이지 정보를 들고옴
	FILE* fp;
	fopen_s(&fp, stage_List[game_Stage], "r"); // 현재 스테이지 txt열기
	char buffer[MAX] = { 0, };

	fread(buffer, 1, 1000, fp); //전체 읽기
	char* str = NULL;
	char* temp = strtok_s(buffer, ",\n", &str); //, \n을 기준으로 문자열 자르기
	int stage_index = 0;

	for (int i = 0; temp != NULL; i++) {
		if(i%2==0)Enemy[enemy_Num].X = atoi(temp);
		else {
			Enemy[enemy_Num].Y = atoi(temp);
			Enemy[enemy_Num].Active = true;
			enemy_Num++;
		}
		temp = strtok_s(NULL, ",\n", &str);
	}

	fclose(fp); //파일 포인터 닫기


	//총알 초기화
	for (int i = 0; i < MAX; i++) {
		Bullet[i].Active = false; //총알
		Enemy_Bullet[i].Active = false; //적 총알
	}
}


//--------------------------------------------------------------------
// 게임 로드
//--------------------------------------------------------------------
void init_Game() {
	//모든 스테이지 정보를 들고옴
	FILE* fp; 
	fopen_s(&fp, "Stage_info.txt", "r"); // stage_info파일을 r(읽기) 모드로 열기
	char buffer[MAX] = { 0, };

	fread(buffer, 1, MAX, fp); //전체 읽기
	char* str = NULL;
	char* temp = strtok_s(buffer, "\n",&str); //\n을 기준으로 문자열 자르기
	int stage_index = 0;

	while (temp !=NULL) {
		strcpy_s(stage_List[stage_index++], MAX, temp);
		temp = strtok_s(NULL, "\n",&str);
	}

	fclose(fp); //파일 포인터 닫기
}