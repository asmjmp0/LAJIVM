//
// Created by jmp0 on 2021/3/18.
//
#include "staticExport.h"
#include <iostream>
#include <cstdarg>

void init_vm(){
    init_all();
    printf("init_all\n");
}
void* execute_code(char* shellcode,int i,...){
    int arg = 0;
    int arg_count = 0;
    va_list arg_ptr;
    va_start(arg_ptr,i);
    while(-1 != arg)
    {
        arg=va_arg(arg_ptr,int); //获取下一个参数
        printf("%d ",arg);
    }
    va_end(arg_ptr); //结束
}


