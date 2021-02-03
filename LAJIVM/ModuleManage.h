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
	signed char id; //模块id
	char *name; //名字最大12个字节
	unsigned ds_seg; //数据段
	unsigned cs_seg; //代码段
	LibFuncList * funclist; //是否为lib 如果不是则此项为null
	ModuleStruct* forward;
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
