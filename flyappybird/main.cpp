#include"flybird.hpp"

int main(void)
{
	//��������Ϸ��ʱ�䣬����ֵΪ����Ϸ�������ĺ�����
	clock_t start, end;
	start = clock();
	test();
	end = clock();
	printf("time=%f\n", ((double)end - start) / CLK_TCK);
	//system("pause");����˴���system ���ʹ�ó��������������������˳���ʱ���򲻻�ֹͣ���ֵĲ���
	return 0;
}