//
// Created by jmp0 on 2021/3/18.
//
#include "staticExport.h"
#include <iostream>
#include <cstdarg>


int arg_arr[6];
int arg_len = 0;
unsigned char * load_data;
int load_index = 0;
size_t vm_code_size;
extern unsigned m_code_length;
unsigned char my_mid;
void init_vmcode(char*);
void do_vm_code();

void init_vm(){
    init_all();
}


void* execute_code(char* shellcode,...){
    int arg = 0;
    va_list arg_ptr;
    va_start(arg_ptr,shellcode);
    int arg_count =va_arg(arg_ptr,int); //获取下一个参数
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
    do_vm_code();
    return nullptr;
}
static int load_data_vm() {
    int data_len;
    if (!bincmp((char *)(load_data+load_index),LVM_SIGNE,BIN_HEAD_LEN))
    {
        std::cout << "不是这个虚拟机的可执行二进制文件" << std::endl;
        throw(LVM_BIN_READ_ERROR);
    }else load_index = BIN_HEAD_LEN;
    //判断是不是库文件
    file_chara = *(load_data + load_index);
    if (!(file_chara == 0x7f || file_chara == 0xff)) {
        std::cout << "不是这个虚拟机的可执行二进制文件" << std::endl;
        throw LVM_BIN_READ_ERROR;
    }
    ++load_index;
    if (!bincmp((char *)(load_data+load_index),LVM_DATA,BIN_DATA_LEN))
    {
        std::cout << "不是这个虚拟机的可执行二进制文件" << std::endl;
        throw(LVM_BIN_READ_ERROR);
    }else load_index += BIN_DATA_LEN;
    data_len = find_str((char *)(load_data + load_index), LVM_CODE, vm_code_size - load_index, BIN_CODE_LEN);
    if (data_len == -1){
        std::cout << "不是这个虚拟机的可执行二进制文件" << std::endl;
        throw(LVM_BIN_READ_ERROR);
    }else
    {
        if (data_len == 0) {
            d_adr = get_memory(4, DataSegType, my_mid);
            return LVM_SUCCESS;
        }
        d_adr=get_memory(data_len, DataSegType,my_mid);
        if(d_adr!=-1) memcpy(data_ptr+d_adr, load_data + load_index, data_len);
        else throw LVM_DATA_GET_ERROR;
    }
    load_index += data_len;
    return LVM_SUCCESS;
}
static int load_code_vm(){
    int code_len;
    load_index += BIN_CODE_LEN;//下标指向代码段数据
    code_len = find_str((char *)(load_data + load_index), LVM_END, vm_code_size - load_index, BIN_END_LEN);
    m_code_length = (unsigned)code_len;
    if (code_len == -1) {
        std::cout << "不是这个虚拟机的可执行二进制文件" << std::endl;
        throw(LVM_BIN_READ_ERROR);
    }else
    {
        c_adr = get_memory(code_len, CodeSegType,my_mid);
        if (c_adr != -1) memcpy(code_ptr+c_adr, load_data + load_index, code_len);
        else throw LVM_CODE_GET_ERROR;
    }
    return LVM_SUCCESS;
}
void init_vmcode(char* shellcode){
   load_data= base64_decode((unsigned char *)shellcode);
   size_t base64_data_len = strlen((char *)shellcode);
   vm_code_size = (base64_data_len/4)*3;
   my_mid= new_module_id();
   load_data_vm();
   load_code_vm();
   load_module( c_adr, "test", my_mid, nullptr, d_adr);
   registe_ptr->CS = c_adr;
   registe_ptr->DS = d_adr;
   load_index = 0;
}

static inline void clean_register(){
    for (int i=0; i<REG_NUM; ++i)
        *(register_list[i])=0;
    registe_ptr->R6 = unsigned(-1);
    registe_ptr->R7 = unsigned(-1);
}

void do_vm_code(){
    for (int i=0; i<arg_len; ++i){
        *(register_list[i]) = (unsigned )arg_arr[i];
    }
    registe_ptr->IP = registe_ptr->IP + registe_ptr->CS;
    try {
        while (registe_ptr->IP<m_code_length+registe_ptr->CS)
            exectue_ins();
    } catch (int e) {
        if(e == 2) {
            kill_module(current_module_id);
            clean_register();
            return;
        }
        else{
            printf("运行过程中出错 %08X\n",e);
            exit(-1);
        }
    }

}

