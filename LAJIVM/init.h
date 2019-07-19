#ifndef INIT_H
#define INIT_H
#include"VmRegister.h"
#include"ExecuteInstruction.h"
extern unsigned m_code_length;
extern char *code_ptr;
extern char *data_ptr;
extern char *stack_ptr;
extern vm_register* registe_ptr;
extern unsigned* register_list[REG_NUM];
extern int(*ins_list[0xff])();
vm_register* init_register();
char* init_stack_segement();
char* init_code_segment();
char* init_data_segement();
int init_list();
void init_all();
#define code_length 0xffff
#define data_length 0xffff
#define stack_length 0xffff
#define bin_path "D:\\tempdata\\bin"
#endif // !INIT_H

