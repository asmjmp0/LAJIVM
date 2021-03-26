//
// Created by jmp0 on 2021/3/18.
//
#include "staticExport.h"
#include <iostream>
#include <cstdarg>


int arg_arr[6];
int arg_len = 0;
void init_vmcode(char*);
void do_vm_code(char*);

void init_vm(){
    init_all();
    printf("init_all\n");
}


void* execute_code(char* shellcode,...){
    int arg = 0;
    va_list arg_ptr;
    va_start(arg_ptr,shellcode);
    int arg_count =va_arg(arg_ptr,int); //获取下一个参数
    printf("%d\n",arg_count);
    if(arg_count > 4 || arg_count <=0){
        printf("%s","the parm count over the max count!");
        return nullptr;
    }
    while (arg_count--)
    {
        arg=va_arg(arg_ptr,int); //获取下一个参数
        arg_arr[arg_len] = arg;
        ++arg_len;
    }
    printf("\n");
    va_end(arg_ptr); //结束

    init_vmcode(shellcode);
    do_vm_code(shellcode);
}

void init_vmcode(char* shellcode){

}

void do_vm_code(char* shellcode){
    for (int i=0; i<arg_len; ++i){
        printf("%d\n",arg_arr[i]);
    }
}

