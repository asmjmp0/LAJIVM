#ifndef MODULEMANAGE_H
#define MODULEMANAGE_H

#define MAX_MODULE_NUM 0xfe
struct LibFuncList
{
	char *namelist;
	int *offsetlist;
	int count;
};
struct ModuleStruct
{
	unsigned char id; //ģ��id
	char *name; //�������12���ֽ�
	unsigned ds_seg; //���ݶ�
	unsigned cs_seg; //�����
	LibFuncList * funclist; //�Ƿ�Ϊlib ������������Ϊnull
	ModuleStruct* forword;
	ModuleStruct* next;
};
extern ModuleStruct *module_head;
extern bool idlist[MAX_MODULE_NUM];
void init_module_manager();
bool load_module(unsigned cs_seg ,char* name, unsigned char id,LibFuncList* lib, unsigned ds_seg);
unsigned new_module_id();
bool kill_module(unsigned char id);
void test_print_module();
#endif // MODULEMANAGE_H
