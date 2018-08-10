#define _CRT_SECURE_NO_WARNINGS 1

#define NEW_OVERLOAD_IMPLEMENTATION_
#include"memory.h"


//初始化刚才在类中定义的静态对象，类中定义，类外初始化
size_t Memory::AllocSize = 0;

//定义双向链表的结点，从而记录申请的内存块
typedef struct MemoryListNode
{
	MemoryListNode* _pre;
	MemoryListNode* _next;
	size_t _size;//申请内存的多少
	char* _file;//存在于哪个文件中
	size_t _line;//哪一行存在内存泄漏
	bool _isArray;//是否申请的为数组
}MemoryListNode;

//构造一个头结点,并且使得这个链表只在本文件中有效,类似数组的初始化,双向链表的头结点指向自己
static MemoryListNode MemoryListLinkpHead = {
	&MemoryListLinkpHead, &MemoryListLinkpHead, 0, NULL, 0, false
};

//保存未释放的内存大小
static size_t NonMemory = 0;

//申请内存
void* AllocMemory(size_t size, char* file, size_t line, bool isArray)
{

	//在为用户申请内存之时，需要为每一个结点也申请内存，从而在释放的时候，将链表的结点也可以释放
	size_t newSize = size + sizeof(MemoryListNode);
	//申请结点，管理内存,严格的内存管理机制
	//此处是在制作c++内存泄漏检测器，因此不能使用new或者delete，使用new与delete底层的malloc与free从而实现内存的管理
	MemoryListNode* Elem = (MemoryListNode*)malloc(newSize);

	//更新链表的指向,头插法
	Elem->_next = MemoryListLinkpHead._next;
	MemoryListLinkpHead._next->_pre = Elem;
	Elem->_pre = &MemoryListLinkpHead;
	MemoryListLinkpHead._next = Elem;

	//更新新结点中的内容
	Elem->_size = size;//结点申请的大小
	Elem->_isArray = isArray;//判断是否为数组形式的空间，默认不是

	//如果存在文件信息，则将文件保存下来
	if (file != NULL)
	{
		////+1是为了保存文件最后的\0
		Elem->_file =(char*)malloc(strlen(file) + 1);
		strcpy(Elem->_file, file);//保存文件
	}
	else
	{
		Elem->_file = NULL;
	}

	Elem->_line = line;

	//更新用户申请的内存大小，此处只是为用户所申请的，与为保存文件信息而申请的内存无关，对于这部分信息，在释放内存之时单独进行释放
	NonMemory += size;
	//返回用户申请内存的初始地址，以及为了保存内存而申请的结点的内存，而将返回的内存强转为char* 为了保证在对内存的使用之时
	//使得++或者--之后，地址向后偏移一个内存的大小，
	//当要释放内存之时，使用一共的内存大小减去结点所占的内存，即为申请的内存
	return ((char*)Elem + sizeof(MemoryListLinkpHead));//表示偏移量
}

//释放内存
void DelMemory(void* ptr, bool isArray)
{
	//先计算出申请内存的多少
	//堆栈相对而生，而堆区向上增长，因此向地址减小的地方增长，因此此处为减
	MemoryListNode* DelNode = (MemoryListNode*)((char*)ptr - sizeof(MemoryListNode));
	//采用双向链表头删的方法删除要申请的结点
	
	//当delete与delete[]没有匹配使用之时，则此时不能释放这样的内存
	if (isArray != DelNode->_isArray)
		return;

	//更新链表的指向
	DelNode->_next->_pre = DelNode->_pre;
	DelNode->_pre->_next = DelNode->_next;

	//更新未释放内存的大小
	NonMemory -= DelNode->_size;
	//删除刚才为保存这个结点文件的信息的内存
	if (DelNode->_file != NULL)
	{
		free(DelNode->_file);
		DelNode->_file = NULL;//防止出现野指针
	}

	//删除申请的内存
	free(DelNode);
	DelNode = NULL;

}

void* operator new(size_t size, char* file, size_t line)
{
	return AllocMemory(size, file, line, false);
}

void* operator new[](size_t size, char* file, size_t line)
{
	return AllocMemory(size, file, line, true);
}

void operator delete(void* ptr)
{
	DelMemory(ptr, false);
}

void operator delete[](void* ptr)
{
	DelMemory(ptr, true);
}

//显示信息
void Memory::_Memory()
{
	
	//当没有残留内存之时，则表示内存释放完成
	if (NonMemory == 0)
	{
		std::cout << "不存在内存泄漏！！" << std::endl;
		return;
	}

	//记录泄漏的内存次数，链表中结点的个数,双向链表
	int count = 0;
	MemoryListNode* cur = MemoryListLinkpHead._next;
	while (cur!=NULL&&cur != &MemoryListLinkpHead)
	{
		if (cur->_isArray == false)
			std::cout << "new 存在内存泄漏" << std::endl;
		else
			std::cout << "new[] 存在内存泄漏" << std::endl;

		if (cur->_file != NULL)
			std::cout << "位于文件" << cur->_file << "中的第" << cur->_line << "行" << std::endl;
		else
			std::cout << "无文件信息" << std::endl;

		std::cout << "指向" << cur << "的内存存在" << cur->_size << "字节的内存泄漏" << std::endl;
		cur = cur->_next;
		count++;
	}

	std::cout << "存在" << count << "次的内存泄漏" << std::endl;
	return;
}
