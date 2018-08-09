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
int high, width;//地图高度与宽度 
int bird_x, bird_y;//小鸟的位置 
int through, wall;//通道的x坐标和墙的起始坐标y 
int map[MAX][MAX];//记录页面的显示 
/*0代表空白，1代表小鸟的位置，2代表墙
3代表上下围墙，4代表左右围墙*/
bool flag[MAX][MAX];//代表改点有围墙
int score;//记录得分 
bool result = 0;//游戏结果1代表失败，0代表胜利
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
//记录数据
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
	//下方围墙的初始化 
	i = high;
	for (j = 1; j <= width; j++)
		map[i][j] = 3;
	//小鸟位置的初始化 
	map[bird_x][bird_y] = 1;

	//墙的初始化
	for (i = 1; i <= high - 1; i++)
	{
		map[i][wall] = 2;
		flag[i][wall] = 1;
	}


	for (i = through - 2; i <= through + 2; i++)//通道的大小可以自定义.
	{
		map[i][wall] = 0;
		flag[i][wall] = 0;
	}

}
//隐藏光标 
void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
//清理屏幕
void gotoxy(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}
//与输入无关的更新 
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
		wall--;//墙的移动。保证形成动态的画面
	}

	bird_x++;//确保鸟可以下降
	
	//bird_x <= 0防止小鸟向上跑的太快
	if (flag[bird_x][bird_y] == 1 || bird_x >= high || bird_x <= 0)//当小鸟死亡时 
	{
		result = 1;
	}
	if (wall == bird_y)
		score++;
	//产生一个随机的墙
	if (wall < 1)
	{
		srand(time(NULL));
		through = rand() % high;//假定墙的高度为20，则空的高度为0~19
		while (through <= 3 || through >= high - 2)//空开始的坐标
		{
			through = rand() % high;
		}
	}
	//下一次墙出现的位置

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
	if (_kbhit())//判断是否有键盘输入 
	{
		input = _getch();
		if (input == 'a')
			bird_x -= 3;//小鸟向上蹦两格
		if (input == 's')
		{
			bird_x += 1;
		}
		if (input == 'p')//实现游戏的暂停
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
	printf("你的分数是:%d\n\n", score);
	printf("操作说明:字符a向上移动\n");
	printf("操作说明:字符s向下移动\n");
	printf("操作说明:字符p暂停游戏\n");
	printf("操作说明:按任意键小鸟继续运动\n");
}

void menu()
{


	printf("\t欢迎来到我的flyappybird\t\n");
	printf("0.退出游戏\n");
	printf("1.开始游戏^^\n");
	printf("请选择：");
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

				//表示失败
				if (result == 1)
					break;

				UPdatewithinput();

			}
			system("cls");
			printf("你输了\n");
			//使得再一次进入的时候，能够正确的开始运行
			result = 0;
			menu();

			//break;
			return;
		default:
			printf("啊哦。选择错误!!!请重新选择\n");
			//如果输入的一个字符或者是一个字符串，则此时这个数据会在缓冲区中暂留，因此将缓冲区清空
			//此种方式用于在cin的输入上
			/*std::cin.clear();
			std::cin.sync(); */  //或者用cin.ignore();

			//c语言风格的清除缓冲区
			fflush(stdin);
			menu();
			return;

		}
	}
}



void testmusic()
{
	//此处只能播放wav格式的音乐
	//PlaySound(ROUTE, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	//播放window系统下自带的声音
	PlaySound("SystemStart", NULL, SND_ALIAS | SND_ASYNC | SND_LOOP);
	//system("F:\\CloudMusic\\当你.mp3");
	std::cin.get();	
}



void createPthread()
{
	std::thread game(menu);
	
	std::thread music(testmusic);

	
	//线程等待
	game.join();

	music.join();

}
void test()
{
	createPthread();
}

