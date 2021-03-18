#include"headers/MemoryManage.h"
#include<stdio.h>
MemoryNode * data_used_memory_head = nullptr;
MemoryNode *data_free_memory_head = nullptr;
MemoryNode *code_used_memory_head = nullptr;
MemoryNode *code_free_memory_head = nullptr;
void init_get_head(int type, MemoryNode *&free, MemoryNode *&used) {
	if (type == 0) {
		used = data_used_memory_head;
		free= data_free_memory_head;
	}
	if (type == 1) {
		used = code_used_memory_head;
		free = code_free_memory_head;
	}
}
void clear_node(MemoryNode * node) {
	node->address = 0;
	node->forward = nullptr;
	node->next = nullptr;
	node->size = 0;
	node->mid = -1;
}
void init_memory_manager() {
	code_used_memory_head = new MemoryNode;
	data_used_memory_head = new MemoryNode;
	code_free_memory_head = new MemoryNode;
	data_free_memory_head = new MemoryNode;
	clear_node(code_free_memory_head);
	clear_node(data_free_memory_head);
	clear_node(code_used_memory_head);
	clear_node(data_used_memory_head);

	/*创建空闲的数据段节点*/
	MemoryNode* first_free = new MemoryNode;
	first_free->address = 0;
	first_free->size = 0xffff;
	first_free->forward = data_free_memory_head;
	first_free->next = nullptr;
	data_free_memory_head->next = first_free;

	/*创建空闲的代码段段节点*/
	first_free = new MemoryNode;
	first_free->address = 0;
	first_free->size = 0xffff;
	first_free->forward = code_free_memory_head;
	first_free->next = nullptr;
	code_free_memory_head->next = first_free;

	data_used_memory_head->address = -1;
	data_used_memory_head->size = -1;
	code_used_memory_head->address = -1;
	code_used_memory_head->size = -1;
}
MemoryNode* find_memory(unsigned len, MemoryNode* free) {
	MemoryNode *p = free;
	while (p)
	{
		if (p->size >= len) return p;
		else p = p->next;
	} 
	return nullptr;
}
MemoryNode* find_last_node(MemoryNode* head) {
	MemoryNode *temp = head;
	while (temp->next) temp = temp->next;
	return temp;
}
unsigned get_memory(unsigned len, int type,unsigned char mid) {
	MemoryNode *used_head = nullptr,*free_head = nullptr;
	if (!len) return-1;
	init_get_head(type, free_head, used_head);
	MemoryNode* p = find_memory(len,free_head);
	if (p != nullptr) {
		/*创建新的节点并把节点插入使用的最后*/
		unsigned adr = p->address;
		MemoryNode* new_used = new MemoryNode;
		new_used->address = p->address;
		new_used->size = len;
		new_used->mid = mid;
		MemoryNode* used_node = find_last_node(used_head);
		used_node->next = new_used;
		new_used->forward = used_node;
		new_used->next = nullptr;
		/*空闲块地址加 大小减小*/
		if (p->size == len) { //大小相等则取下节点
			MemoryNode *p_forword = p->forward;
			MemoryNode *p_next = p->next;
			p_forword->next = p_next;
			p_next->forward = p_forword;
		}
		else {
			p->address += len;
			p->size -= len;
		}
		return adr;
	}else return -1;
}
bool free_memory(unsigned adr,int type) {
	MemoryNode *used_head=nullptr, *free_head = nullptr;
	init_get_head(type, free_head, used_head);
	MemoryNode * used = used_head;
	unsigned adr_size;
	while (used) //获取使用的链表
	{
		if (used->address == adr) break;
		else used = used->next;
	}
	if (!used) return false;
	else
	{
		adr_size = used->size;
		/*从使用链表中取下*/
		MemoryNode *forword = used->forward;
		MemoryNode *next = used->next;
		forword->next = used->next;
		if (next) next->forward = forword;
		delete(used);

		/*加入未使用的节点采用头插入*/

		MemoryNode *data_first = free_head->next;
		MemoryNode *new_free = new MemoryNode;
		new_free->address = adr;
		new_free->size = adr_size;
		new_free->forward = free_head;
		new_free->next = data_first;
		free_head->next = new_free;
		data_first->forward = new_free;

		return true;
	}
}
void test_memory_print(int type) {
	char *str=nullptr;
	char code_str[5]="code";
	char data_str[5] = "data";
	MemoryNode *used_head = nullptr, *free_head = nullptr;
	init_get_head(type, free_head, used_head);
	if (type == 0) str = data_str;
	else if (type == 1) str = code_str;
	MemoryNode *p = nullptr;
	printf("-------%s_free_memory link-------\n",str);
	p = free_head;
	while (p)
	{
		printf("address:%08x\n", p->address);
		printf("size:%08x\n", p->size);
		printf("\n");
		p = p->next;
	}
	printf("\n\n\n");
	printf("-------%s_used_memory link-------\n",str);
	p = used_head;
	while (p)
	{
		printf("address:%08x\n", p->address);
		printf("size:%08x\n", p->size);
		printf("\n");
		p = p->next;
	}
}