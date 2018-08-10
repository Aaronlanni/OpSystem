#pragma once

#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<Windows.h>
//c++内存检测器，c++中的动态内存开辟使用new 与delete ，当检测new申请的内存是否全部释放，如果全部释放，则没有内存泄漏，
//如果没有全部释放
//则存在内存泄漏，此时，需要记载内存泄漏的地方以及内存泄露存在于哪个文件中，将其信息打印出来，从而完成内存泄漏的检测
//此时使用双向循环链表将其申请的内存块连接起来，不使用单链表

//参数一表示申请的内存的大小。第二个参数表示内存泄漏等的文件，第三个参数表示内存泄漏存在于第几行
void* operator new(size_t size, char* file, size_t line);
void* operator new[](size_t size, char* file, size_t line);

//参数表示指向释放内存的空间
void operator delete(void* ptr);
void operator delete[](void* ptr);


#ifndef NEW_OVERLOAD_IMPLEMENTATION_
#define new new( __FILE__, __LINE__ )
//条件编译
// __FILE__(两个下划线): 代表当前源代码文件名的字符串文字(我们用这个宏获得存在内存泄漏文件的文件名)
// __LINE__(两个下划线): 代表当前源代码文件中的行号的整数常量(我们用这个宏获得存在内存泄漏文件内存泄漏的行号)

#endif

//定义一个类，从而用来检测我们的内存是否存在泄漏
class Memory
{
public:
	static size_t AllocSize;//静态申请的变量，为每一个对象所共享，链接属性为内链接，因此不会因为出现在多个源文件中，从而出现重复定义的现象

	//构造函数
	Memory()
	{
		++AllocSize;
	}
	~Memory()
	{
		if (0 == --AllocSize)
		{
			_Memory();//当构造对象之时，就需要使得AllocSize加1，因此，当最后只有一个头结点之时，将其释放掉
			
			Sleep(100000);//当进程退出的时候，静态对象就会立马被销毁，因此当进程退出那一刻，会出现程序是否泄漏的报告，而在vs的平台下，会出现闪退的情况，因此需要将其程序在运行多久之后退出
		}
	}
private:
	void _Memory();
};

static Memory exitCount;



