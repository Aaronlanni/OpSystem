#define _CRT_SECURE_NO_WARNINGS 1

#include"memory.h"
#include<Windows.h>
void test()
{
	int* a = new int[8];
	double* b = new double;
	delete a;
	delete b;
}

int main()
{
	test();
	
	//system("pause");
	
	return 0;
}
