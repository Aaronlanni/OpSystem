#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<windows.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#include<thread>

#pragma comment(lib,"WinMM.Lib")
#define MAX 100
int high, width;//��ͼ�߶����� 
int bird_x, bird_y;//С���λ�� 
int through, wall;//ͨ����x�����ǽ����ʼ����y 
int map[MAX][MAX];//��¼ҳ�����ʾ 
/*0����հף�1����С���λ�ã�2����ǽ
3��������Χǽ��4��������Χǽ*/
bool flag[MAX][MAX];//����ĵ���Χǽ
int score;//��¼�÷� 
bool result = 0;//��Ϸ���1����ʧ�ܣ�0����ʤ��
int speed;

#define ROUTE "E:\\chimes.wav"

void menu();
void startup()
{
	score = 0;
	high = 20;
	width = 50;
	bird_x = high / 2;
	bird_y = width / 5;
	through = high / 2;
	wall = width / 4 * 3;
	speed = 0;
}
//��¼����
void startMap()
{
	int i, j;
	for (i = 1; i <= high - 1; i++)
	{
		map[i][1] = 4;
		for (j = 2; j <= width - 1; j++)
			map[i][j] = 0;
		map[i][width] = 4;
	}
	//�·�Χǽ�ĳ�ʼ�� 
	i = high;
	for (j = 1; j <= width; j++)
		map[i][j] = 3;
	//С��λ�õĳ�ʼ�� 
	map[bird_x][bird_y] = 1;

	//ǽ�ĳ�ʼ��
	for (i = 1; i <= high - 1; i++)
	{
		map[i][wall] = 2;
		flag[i][wall] = 1;
	}


	for (i = through - 2; i <= through + 2; i++)//ͨ���Ĵ�С�����Զ���.
	{
		map[i][wall] = 0;
		flag[i][wall] = 0;
	}

}
//���ع�� 
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
//������Ļ
void gotoxy(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}
//�������޹صĸ��� 
void UPdatewithOutinput()
{

	int i = 20;
	int j = 2;
	if (score >= i)
	{
		i += 2;
		wall -= j;
		++j;
		speed++;
		if (j > (width / 4 * 3))
		{
			j = 1;
		}
	}
	else
	{
		wall--;//ǽ���ƶ�����֤�γɶ�̬�Ļ���
	}

	bird_x++;//ȷ��������½�
	
	//bird_x <= 0��ֹС�������ܵ�̫��
	if (flag[bird_x][bird_y] == 1 || bird_x >= high || bird_x <= 0)//��С������ʱ 
	{
		result = 1;
	}
	if (wall == bird_y)
		score++;
	//����һ�������ǽ
	if (wall < 1)
	{
		srand(time(NULL));
		through = rand() % high;//�ٶ�ǽ�ĸ߶�Ϊ20����յĸ߶�Ϊ0~19
		while (through <= 3 || through >= high - 2)//�տ�ʼ������
		{
			through = rand() % high;
		}
	}
	//��һ��ǽ���ֵ�λ��

	if (wall < 1)
	{
		wall = width / 4 * 3;
	}
	memset(flag, 0, sizeof(flag));
	Sleep(100);
}
void UPdatewithinput()
{
	char input;
	if (_kbhit())//�ж��Ƿ��м������� 
	{
		input = _getch();
		if (input == 'a')
			bird_x -= 3;//С�����ϱ�����
		if (input == 's')
		{
			bird_x += 1;
		}
		if (input == 'p')//ʵ����Ϸ����ͣ
		{
			system("pause");
		}
	}
}
void show()
{
	gotoxy(0, 0);
	int i, j;
	for (i = 1; i <= high; i++)
	{
		for (j = 1; j <= width; j++)
		{
			switch (map[i][j])
			{
			case 0:
				printf(" ");
				break;
			case 1:
				printf(">");
				break;
			case 2:
				printf("$");
				break;
			case 3:
				printf("-");
				break;
			case 4:
				printf("|");
				break;
			}
		}
		printf("\n");
	}
	printf("��ķ�����:%d\n\n", score);
	printf("����˵��:�ַ�a�����ƶ�\n");
	printf("����˵��:�ַ�s�����ƶ�\n");
	printf("����˵��:�ַ�p��ͣ��Ϸ\n");
	printf("����˵��:�������С������˶�\n");
}

void menu()
{


	printf("\t��ӭ�����ҵ�flyappybird\t\n");
	printf("0.�˳���Ϸ\n");
	printf("1.��ʼ��Ϸ^^\n");
	printf("��ѡ��");
	while (1)
	{
		int i;
		scanf("%d", &i);
		//std::cin >> i;

		switch (i)
		{
		case 0:
			return;
		case 1:
			startup();

			while (1)
			{
				HideCursor();
				startMap();
				show();
				UPdatewithOutinput();

				//��ʾʧ��
				if (result == 1)
					break;

				UPdatewithinput();

			}
			system("cls");
			printf("������\n");
			//ʹ����һ�ν����ʱ���ܹ���ȷ�Ŀ�ʼ����
			result = 0;
			menu();

			//break;
			return;
		default:
			printf("��Ŷ��ѡ�����!!!������ѡ��\n");
			//��������һ���ַ�������һ���ַ��������ʱ������ݻ��ڻ���������������˽����������
			//���ַ�ʽ������cin��������
			/*std::cin.clear();
			std::cin.sync(); */  //������cin.ignore();

			//c���Է������������
			fflush(stdin);
			menu();
			return;

		}
	}
}



void testmusic()
{
	//�˴�ֻ�ܲ���wav��ʽ������
	//PlaySound(ROUTE, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//����windowϵͳ���Դ�������
	PlaySound("SystemStart", NULL, SND_ALIAS | SND_ASYNC | SND_LOOP);
	//system("F:\\CloudMusic\\����.mp3");
	std::cin.get();	
}



void createPthread()
{
	std::thread game(menu);
	
	std::thread music(testmusic);

	
	//�̵߳ȴ�
	game.join();

	music.join();

}
void test()
{
	createPthread();
}

