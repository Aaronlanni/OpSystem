#include"flybird.hpp"

int main(void)
{
	//返回玩游戏的时间，返回值为玩游戏所经历的毫秒数
	clock_t start, end;
	start = clock();
	test();
	end = clock();
	printf("time=%f\n", ((double)end - start) / CLK_TCK);
	//system("pause");如果此处加system 则会使得程序阻塞，导致是正常退出的时候，则不会停止音乐的播放
	return 0;
}