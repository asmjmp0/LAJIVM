#include"ModuleManage.h"
#include"MemoryManage.h"
#include<iostream>
ModuleStruct *module_head=nullptr;
bool idlist[MAX_MODULE_NUM] = { false };
unsigned new_module_id() {
	for (unsigned i = 0; i < 0xff; ++i) if (!idlist[i]) {
		idlist[i] = true;
		return i;
	}
	return -1;
}
void init_module_manager() {
	module_head = new ModuleStruct;
	module_head->id = -1;
	module_head->name = nullptr;
	module_head->cs_seg = 0;
	module_head->ds_seg = 0;
	module_head->forward = nullptr;
	module_head->next = nullptr;
	module_head->funclist = nullptr;
}
bool load_module( unsigned cs_seg, char* name,  unsigned char id, LibFuncList* lib = nullptr, unsigned ds_seg=-1) {
	/*创建新节点并写入参数*/
	ModuleStruct *new_module = new ModuleStruct;
	new_module->cs_seg = cs_seg;
	new_module->ds_seg = ds_seg;
	new_module->name = name;
	new_module->funclist = lib;
	new_module->id = id;
	if (new_module->id == -1) return false;

	/*把节点加到最后*/
	ModuleStruct *final_node = module_head;
	while (final_node->next) final_node = final_node->next;
	final_node->next = new_module;
	new_module->forward = final_node;
	new_module->next = nullptr;

	return true;
}
bool kill_module(unsigned char id) {//删除一个模块
	if (!idlist[id]) return false;
	else
	{
		/*找到节点*/
		ModuleStruct *being_kill_mod=module_head;
		while (being_kill_mod->id != id) being_kill_mod = being_kill_mod->next;
		ModuleStruct *forword = being_kill_mod->forward;
		/*取下节点*/
		if (being_kill_mod->next) {//next不为空
			forword->next = being_kill_mod->next;
			being_kill_mod->next->forward = forword;
		} else forword->next = nullptr;
		
		/*释放资源*/
		free_memory(being_kill_mod->cs_seg, CodeSegType);
		free_memory(being_kill_mod->ds_seg, DataSegType);

		MemoryNode *data_used = data_used_memory_head;
		while (data_used)
		{
			if (data_used->mid = id) free_memory(data_used->address,DataSegType);
			data_used = data_used->next;
		}
		if (!being_kill_mod->funclist) delete(being_kill_mod->funclist);
		delete(being_kill_mod);
		return true;
	}
}
void test_print_module() {
	ModuleStruct *temp=module_head;
	printf("-------module link-------\n");
	while (temp)
	{
		printf("id: %d\n",temp->id);
		printf("cs: %d\n", temp->cs_seg);
		printf("ds: %d\n", temp->ds_seg);
		printf("name: %s\n", temp->name);
		printf("\n");
		temp = temp->next;
	}
}
