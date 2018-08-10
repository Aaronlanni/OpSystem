#pragma once

#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<Windows.h>
//c++�ڴ�������c++�еĶ�̬�ڴ濪��ʹ��new ��delete �������new������ڴ��Ƿ�ȫ���ͷţ����ȫ���ͷţ���û���ڴ�й©��
//���û��ȫ���ͷ�
//������ڴ�й©����ʱ����Ҫ�����ڴ�й©�ĵط��Լ��ڴ�й¶�������ĸ��ļ��У�������Ϣ��ӡ�������Ӷ�����ڴ�й©�ļ��
//��ʱʹ��˫��ѭ��������������ڴ��������������ʹ�õ�����

//����һ��ʾ������ڴ�Ĵ�С���ڶ���������ʾ�ڴ�й©�ȵ��ļ���������������ʾ�ڴ�й©�����ڵڼ���
void* operator new(size_t size, char* file, size_t line);
void* operator new[](size_t size, char* file, size_t line);

//������ʾָ���ͷ��ڴ�Ŀռ�
void operator delete(void* ptr);
void operator delete[](void* ptr);


#ifndef NEW_OVERLOAD_IMPLEMENTATION_
#define new new( __FILE__, __LINE__ )
//��������
// __FILE__(�����»���): ����ǰԴ�����ļ������ַ�������(������������ô����ڴ�й©�ļ����ļ���)
// __LINE__(�����»���): ����ǰԴ�����ļ��е��кŵ���������(������������ô����ڴ�й©�ļ��ڴ�й©���к�)

#endif

//����һ���࣬�Ӷ�����������ǵ��ڴ��Ƿ����й©
class Memory
{
public:
	static size_t AllocSize;//��̬����ı�����Ϊÿһ��������������������Ϊ�����ӣ���˲�����Ϊ�����ڶ��Դ�ļ��У��Ӷ������ظ����������

	//���캯��
	Memory()
	{
		++AllocSize;
	}
	~Memory()
	{
		if (0 == --AllocSize)
		{
			_Memory();//���������֮ʱ������Ҫʹ��AllocSize��1����ˣ������ֻ��һ��ͷ���֮ʱ�������ͷŵ�
			
			Sleep(100000);//�������˳���ʱ�򣬾�̬����ͻ��������٣���˵������˳���һ�̣�����ֳ����Ƿ�й©�ı��棬����vs��ƽ̨�£���������˵�����������Ҫ������������ж��֮���˳�
		}
	}
private:
	void _Memory();
};

static Memory exitCount;



