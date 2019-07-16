#ifndef INIT_H
#define INIT_H
#include"VmRegister.h"
extern unsigned m_code_length;
extern char *code_ptr;
extern char *data_ptr;
extern vm_register* registe_ptr;
vm_register* init_register();
int init_stack_segement();
char* init_code_segment();
char* init_data_segement();
void init_all();
#define code_length 0xffff
#define data_length 0xffff
#define stack_lenght 0xffff
#define bin_path "D:\\tempdata\\bin"
#endif // !INIT_H

