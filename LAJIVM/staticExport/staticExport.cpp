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
    va_end(arg_ptr); //结束

    init_vmcode(shellcode);
    do_vm_code(shellcode);
    return nullptr;
}

void init_vmcode(char* shellcode){
    //todo:完成vm初始化
   unsigned char* str = base64_encode((unsigned char*)"1234\0001234",9);
   printf("%s\n",(char *)str);
   str = base64_decode(str);
   printf("%s",(char *)str);
//   unsigned char * base64_data = base64_decode((unsigned char *)origin_bin);
//   size_t base64_data_len = strlen((char *)origin_bin);
//   file_size = (base64_data_len/4)*3;
//   memcpy(origin_bin,base64_data,file_size);

}

void do_vm_code(char* shellcode){
    for (int i=0; i<arg_len; ++i){
        printf("%d\n",arg_arr[i]);
    }
}

