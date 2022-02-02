#include <stdio.h>
#include <memory.h>
#include <Windows.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include "Console.h"

//���� ����
char szScreenBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];
char frontBuffer[SCREEN_HEIGHT][SCREEN_WIDTH];

const int MAX = 100;
int iX, iY; //ĳ���� ��ǥ
int b_index; //�Ѿ� �迭 index
int eb_index; //�� �Ѿ� �迭 index

char stage_List[MAX][MAX]; // �������� ����Ʈ 
int enemy_Num = 0; // �� ����
int game_Score = 0; // ���� ����
int game_Status = 1; // ���� ���� (0 = ������, 1 = GameOver, 2 = Stage Clear 4 = Stage End)
int game_Stage; // ��������

struct Point { //��ǥ
	int X = 0;
	int Y = 0;
	bool Active = false;
};
Point Enemy[MAX]; //��
Point Bullet[MAX]; //�Ѿ�
Point Enemy_Bullet[MAX]; //�� �Ѿ�

//�Լ� Set
void Buffer_Flip(void);
void Buffer_Clear(void);
void Sprite_Draw(int iX, int iY, wchar_t chSprite);
void Buffer_Print(int j, int i, char c);
void Bullet_Shoot();
void Charector_Move();
void Enemy_Create(); //�� ����
void Enemy_Attack(); //�� ����
void Enemy_Move(); //�� ������
void ShowOption(); //����â ǥ��
void init_Stage(); //�������� �ε�
void init_Game(); //���� �ε�

//Main �Լ� ����
void main(void)
{
	cs_Initial();
	game_Start();
	init_Game();

	while (game_Status == 1) {
		Buffer_Clear();
		init_Stage();

		//�������� ����
		while (game_Status == 0)
		{
			// ��ũ�� ���۸� ����
			Buffer_Clear();

			//����â ǥ��
			ShowOption();

			// ĳ���� ����
			Charector_Move();

			//�Ѿ� �߻�
			Bullet_Shoot();

			//�� ǥ��
			Enemy_Create();
			Enemy_Move();
			Enemy_Attack();

			// ��ũ�� ���۸� ȭ������ ���
			Buffer_Flip();

			// ������ ���߱�� ��� 10 Frame
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
// ���� ���ۿ� ���� ���۸� ���ϴ� �Լ�
//
// szScreenBuffer�� front_buffer�� ���Ͽ� �ٸ��ٸ� Buffer_Print �Լ��� ȣ���Ѵ�.
//--------------------------------------------------------------------
void Buffer_Flip(void)
{
	//���� szScreenBuffer�� front_buffer ��
	for (int i = 0; i < SCREEN_HEIGHT; ++i) {
		for (int j = 0; j < SCREEN_WIDTH; ++j)
		{
			if (frontBuffer[i][j] != szScreenBuffer[i][j])
			{
				Buffer_Print(j, i, szScreenBuffer[i][j]);
				// �ٲ� �κ� ȭ�鿡 ���
				frontBuffer[i][j] = szScreenBuffer[i][j];
				// �ٲ� �κ��� ��� �� front_buffer�� ����
			}
		}
		SetConsoleActiveScreenBuffer(szScreenBuffer[i]);
	}
}


//--------------------------------------------------------------------
// ȭ�� ���۸� �����ִ� �Լ�
//
// �� ������ �׸��� �׸��� ������ ���۸� ���� �ش�. 
// �ȱ׷��� ���� �������� �ܻ��� �����ϱ�
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
// ĳ������ ��ǥ�� �̵�
//
// ����Ű�� �Է¹޾� X,Y��ǥ�� �̵���Ų��.
//--------------------------------------------------------------------
void Charector_Move() {

	if (GetAsyncKeyState(VK_LEFT)) { //����
		iX--;
		if (iX < 0) iX = 0;
	}
	if (GetAsyncKeyState(VK_RIGHT)) { //������
		iX++;
		if (iX > 78) iX = 78;
	}
	if (GetAsyncKeyState(VK_UP)) { //��
		iY--;
		if (iY < 0) iY = 0;
	}
	if (GetAsyncKeyState(VK_DOWN)) { //�Ʒ�
		iY++;
		if (iY > 23) iY = 23;
	}if (GetAsyncKeyState(VK_CONTROL)) { //Ctrl ����
		b_index = 0;
	}
	// ��ũ�� ���ۿ� ĳ���� ���
	szScreenBuffer[iY][iX] = 48;

}

//--------------------------------------------------------------------
// �Ѿ˹߻�� �Ѿ��� ��ǥ�� �̵�, �Ѿ˰� �� �浹
//
// SpaceȮ�� �� �Ѿ� �߻�� ��ǥ�� ��ĭ�� ���� �̵���Ų��.
//--------------------------------------------------------------------
void Bullet_Shoot() {
	
	//�Ѿ� �̵�
	for (int i = 0; i < MAX; i++) {
		if (Bullet[i].Active) {
			Bullet[i].Y--;
			szScreenBuffer[Bullet[i].Y][Bullet[i].X] = 49;

			if (Bullet[i].Y < 1) {
				Bullet[i].Active = false;
			}

			//�Ѿ� �� �浹
			for (int j = 0; j < MAX; j++) {
				if (Enemy[j].Active && Enemy[j].X == Bullet[i].X && Enemy[j].Y == Bullet[i].Y) {
					Enemy[j].Active = false; //�� ���� ��Ȱ��ȭ
					Bullet[i].Active = false; //�Ѿ� ���� ��Ȱ��ȭ
					game_Score += 1000; //���� +1000
					enemy_Num--; //�� ���� -1
				}
			}
		}
	}

	//�Ѿ� �߻�
	if (GetAsyncKeyState(VK_SPACE)) { //SPACE
		if (b_index > 10)return;
		Bullet[b_index].X = iX;
		Bullet[b_index].Y = iY - 1;
		Bullet[b_index].Active = true;
		b_index++;
	}
}

//--------------------------------------------------------------------
// �� ����
//--------------------------------------------------------------------
void Enemy_Create() {
	for (int i = 0; i < MAX; i++) {
		if (Enemy[i].Active) {
			szScreenBuffer[Enemy[i].Y][Enemy[i].X] = 50;
		}
	}
}
//--------------------------------------------------------------------
// ���� ������ �Լ� 
// �� �̻� �����ϰ��� ������ �ݴ�������� ������
// �װ��� �ƴ϶�� �������� ������
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
// ���� �Ѿ� ������
//
// ���� �Ѿ� �߻�� ��ǥ�� ��ĭ�� �Ʒ��� �̵���Ų��.
//--------------------------------------------------------------------
void Enemy_Attack() {
	
	//�� �Ѿ� �̵�
	for (int i = 0; i < MAX; i++) {
		if (Enemy_Bullet[i].Active) {
			Enemy_Bullet[i].Y++;
			szScreenBuffer[Enemy_Bullet[i].Y][Enemy_Bullet[i].X] = 51;

			if (Enemy_Bullet[i].Y >= 23) {
				Enemy_Bullet[i].Active = false;
			}

			//�� �Ѿ˰� ĳ������ �浹 Ȯ��
			if (iX == Enemy_Bullet[i].X && iY == Enemy_Bullet[i].Y) {
				game_Status = 2;
				game_Over();
			}
		}
	}

	if (rand() % 10 != 0)return; //�߻�� Ȯ��
	if (eb_index == MAX-1)eb_index = 0; //�Ѿ� �ʱ�ȭ
	//�� �Ѿ� �߻�
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
// ����, �Ѿ�, ��������, ���� �� �� �� ���� (üũ �� ǥ��)
//--------------------------------------------------------------------
void ShowOption() {
	game_Score++;

	cs_MoveCursor(1, 25);// Ŀ���� 1, 25 ��ġ��
	setColor(WHITE);
	printf("�Ѿ� : ");
	for (int i = 0; i <= 10; i++) {
		if (b_index-1 >= i) {
			setColor(WHITE);
		}
		else {
			setColor(VIOLET);
		}
		printf("��");
	}

	setColor(WHITE);
	cs_MoveCursor(61, 25); // Ŀ���� 60, 25 ��ġ��
	printf("�̵� : ��,��,��,��");
	cs_MoveCursor(61, 26);
	printf("���� : Space");
	cs_MoveCursor(61, 27);
	printf("���� : Ctrl");
	cs_MoveCursor(1, 27);
	printf("���� : %d", game_Score);
	cs_MoveCursor(68, 0); // Ŀ���� 60, 25 ��ġ��
	
	char* str = NULL;
	printf("��������: %d", game_Stage + 1);

	if (enemy_Num == 0) {
		game_Status = 1; //�� ��� ���� �� �������� Clear
	}
}

//--------------------------------------------------------------------
// �������� �ε�
//--------------------------------------------------------------------
void init_Stage() {

	if (strcmp(stage_List[game_Stage],"")==0) { //���������� ���� ��� ����
		game_Status = 4;
		game_End();
		return;
	}

	iX = 40, iY = 23; //ĳ���� ��ǥ
	b_index = 0; //�Ѿ� �迭 index
	eb_index = 0; //�� �Ѿ� �迭 index
	game_Status = 0; //���� ����
	enemy_Num = 0; //�� ����
	
	//��� �������� ������ ����
	FILE* fp;
	fopen_s(&fp, stage_List[game_Stage], "r"); // ���� �������� txt����
	char buffer[MAX] = { 0, };

	fread(buffer, 1, 1000, fp); //��ü �б�
	char* str = NULL;
	char* temp = strtok_s(buffer, ",\n", &str); //, \n�� �������� ���ڿ� �ڸ���
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

	fclose(fp); //���� ������ �ݱ�


	//�Ѿ� �ʱ�ȭ
	for (int i = 0; i < MAX; i++) {
		Bullet[i].Active = false; //�Ѿ�
		Enemy_Bullet[i].Active = false; //�� �Ѿ�
	}
}


//--------------------------------------------------------------------
// ���� �ε�
//--------------------------------------------------------------------
void init_Game() {
	//��� �������� ������ ����
	FILE* fp; 
	fopen_s(&fp, "Stage_info.txt", "r"); // stage_info������ r(�б�) ���� ����
	char buffer[MAX] = { 0, };

	fread(buffer, 1, MAX, fp); //��ü �б�
	char* str = NULL;
	char* temp = strtok_s(buffer, "\n",&str); //\n�� �������� ���ڿ� �ڸ���
	int stage_index = 0;

	while (temp !=NULL) {
		strcpy_s(stage_List[stage_index++], MAX, temp);
		temp = strtok_s(NULL, "\n",&str);
	}

	fclose(fp); //���� ������ �ݱ�
}