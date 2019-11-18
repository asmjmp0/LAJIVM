#ifndef MEMORYMANAGE_H
#define MEMORYMANAGE_H
//�������
//˫������
struct MemoryNode
{
	unsigned char mid;
	unsigned address;
	unsigned size;
	MemoryNode *forword;
	MemoryNode *next;
};
/*�궨��*/
#define DataSegType 0
#define CodeSegType 1
/*type
0��ʾ����
1��ʾ����
*/
//���ݿ�ʹ����Ľڵ�ͷָ��
extern MemoryNode *code_free_memory_head;
extern MemoryNode *data_free_memory_head;
extern MemoryNode *code_used_memory_head;
extern MemoryNode *data_used_memory_head;
void init_memory_manager();
MemoryNode* find_memory(unsigned, MemoryNode*);
MemoryNode* find_last_node(MemoryNode*);
void clear_node(MemoryNode*);
unsigned get_memory(unsigned len,int type,unsigned char mid);
bool free_memory(unsigned adr, int type);
void test_memory_print(int type);
void init_get_head(int type, MemoryNode *&free, MemoryNode *&used);
#endif // !MEMORYMANAGE_H

