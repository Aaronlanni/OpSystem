#define _CRT_SECURE_NO_WARNINGS 1

#define NEW_OVERLOAD_IMPLEMENTATION_
#include"memory.h"


//��ʼ���ղ������ж���ľ�̬�������ж��壬�����ʼ��
size_t Memory::AllocSize = 0;

//����˫������Ľ�㣬�Ӷ���¼������ڴ��
typedef struct MemoryListNode
{
	MemoryListNode* _pre;
	MemoryListNode* _next;
	size_t _size;//�����ڴ�Ķ���
	char* _file;//�������ĸ��ļ���
	size_t _line;//��һ�д����ڴ�й©
	bool _isArray;//�Ƿ������Ϊ����
}MemoryListNode;

//����һ��ͷ���,����ʹ���������ֻ�ڱ��ļ�����Ч,��������ĳ�ʼ��,˫�������ͷ���ָ���Լ�
static MemoryListNode MemoryListLinkpHead = {
	&MemoryListLinkpHead, &MemoryListLinkpHead, 0, NULL, 0, false
};

//����δ�ͷŵ��ڴ��С
static size_t NonMemory = 0;

//�����ڴ�
void* AllocMemory(size_t size, char* file, size_t line, bool isArray)
{

	//��Ϊ�û������ڴ�֮ʱ����ҪΪÿһ�����Ҳ�����ڴ棬�Ӷ����ͷŵ�ʱ�򣬽�����Ľ��Ҳ�����ͷ�
	size_t newSize = size + sizeof(MemoryListNode);
	//�����㣬�����ڴ�,�ϸ���ڴ�������
	//�˴���������c++�ڴ�й©���������˲���ʹ��new����delete��ʹ��new��delete�ײ��malloc��free�Ӷ�ʵ���ڴ�Ĺ���
	MemoryListNode* Elem = (MemoryListNode*)malloc(newSize);

	//���������ָ��,ͷ�巨
	Elem->_next = MemoryListLinkpHead._next;
	MemoryListLinkpHead._next->_pre = Elem;
	Elem->_pre = &MemoryListLinkpHead;
	MemoryListLinkpHead._next = Elem;

	//�����½���е�����
	Elem->_size = size;//�������Ĵ�С
	Elem->_isArray = isArray;//�ж��Ƿ�Ϊ������ʽ�Ŀռ䣬Ĭ�ϲ���

	//��������ļ���Ϣ�����ļ���������
	if (file != NULL)
	{
		////+1��Ϊ�˱����ļ�����\0
		Elem->_file =(char*)malloc(strlen(file) + 1);
		strcpy(Elem->_file, file);//�����ļ�
	}
	else
	{
		Elem->_file = NULL;
	}

	Elem->_line = line;

	//�����û�������ڴ��С���˴�ֻ��Ϊ�û�������ģ���Ϊ�����ļ���Ϣ��������ڴ��޹أ������ⲿ����Ϣ�����ͷ��ڴ�֮ʱ���������ͷ�
	NonMemory += size;
	//�����û������ڴ�ĳ�ʼ��ַ���Լ�Ϊ�˱����ڴ������Ľ����ڴ棬�������ص��ڴ�ǿתΪchar* Ϊ�˱�֤�ڶ��ڴ��ʹ��֮ʱ
	//ʹ��++����--֮�󣬵�ַ���ƫ��һ���ڴ�Ĵ�С��
	//��Ҫ�ͷ��ڴ�֮ʱ��ʹ��һ�����ڴ��С��ȥ�����ռ���ڴ棬��Ϊ������ڴ�
	return ((char*)Elem + sizeof(MemoryListLinkpHead));//��ʾƫ����
}

//�ͷ��ڴ�
void DelMemory(void* ptr, bool isArray)
{
	//�ȼ���������ڴ�Ķ���
	//��ջ��Զ���������������������������ַ��С�ĵط���������˴˴�Ϊ��
	MemoryListNode* DelNode = (MemoryListNode*)((char*)ptr - sizeof(MemoryListNode));
	//����˫������ͷɾ�ķ���ɾ��Ҫ����Ľ��
	
	//��delete��delete[]û��ƥ��ʹ��֮ʱ�����ʱ�����ͷ��������ڴ�
	if (isArray != DelNode->_isArray)
		return;

	//���������ָ��
	DelNode->_next->_pre = DelNode->_pre;
	DelNode->_pre->_next = DelNode->_next;

	//����δ�ͷ��ڴ�Ĵ�С
	NonMemory -= DelNode->_size;
	//ɾ���ղ�Ϊ�����������ļ�����Ϣ���ڴ�
	if (DelNode->_file != NULL)
	{
		free(DelNode->_file);
		DelNode->_file = NULL;//��ֹ����Ұָ��
	}

	//ɾ��������ڴ�
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

//��ʾ��Ϣ
void Memory::_Memory()
{
	
	//��û�в����ڴ�֮ʱ�����ʾ�ڴ��ͷ����
	if (NonMemory == 0)
	{
		std::cout << "�������ڴ�й©����" << std::endl;
		return;
	}

	//��¼й©���ڴ�����������н��ĸ���,˫������
	int count = 0;
	MemoryListNode* cur = MemoryListLinkpHead._next;
	while (cur!=NULL&&cur != &MemoryListLinkpHead)
	{
		if (cur->_isArray == false)
			std::cout << "new �����ڴ�й©" << std::endl;
		else
			std::cout << "new[] �����ڴ�й©" << std::endl;

		if (cur->_file != NULL)
			std::cout << "λ���ļ�" << cur->_file << "�еĵ�" << cur->_line << "��" << std::endl;
		else
			std::cout << "���ļ���Ϣ" << std::endl;

		std::cout << "ָ��" << cur << "���ڴ����" << cur->_size << "�ֽڵ��ڴ�й©" << std::endl;
		cur = cur->_next;
		count++;
	}

	std::cout << "����" << count << "�ε��ڴ�й©" << std::endl;
	return;
}
