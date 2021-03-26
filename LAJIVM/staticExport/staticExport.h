//
// Created by jmp0 on 2021/3/18.
//

#ifndef LAJIVM_STATICEXPORT_H
#define LAJIVM_STATICEXPORT_H
#include "../headers/init.h"
#include "../headers/base64.h"

extern unsigned int ret_value;
extern void *ret_value_pointer;

//初始化虚拟机
void init_vm();

//执行指令
void* execute_code(char*,...);

#endif //LAJIVM_STATICEXPORT_H
