#include"headers/init.h"
#include"headers/ErrorList.h"
#include"headers/ExecuteInstruction.h"
#include<iostream>
int do_push() {
	registe_ptr->SP += 4;//栈指针增加
	if (code_ptr[registe_ptr->IP + 1] == 5) {//寄存器压栈
		uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
		unsigned num = *register_list[high];
		*(unsigned*)(stack_ptr + registe_ptr->SP) = num;
	}
	else if (code_ptr[registe_ptr->IP + 1] == 6) {//立即数压栈
		unsigned num = *(unsigned*)(code_ptr + registe_ptr->IP + 2);
		*(unsigned*)(stack_ptr + registe_ptr->SP) = num;
	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
int do_pop() {
	
	unsigned num = *(unsigned*)(stack_ptr + registe_ptr->SP);//获取栈顶值
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 1] / 0x10;//获取寄存编号
	*register_list[high] = num;//寄存器赋值

	registe_ptr->SP -= 4;
	return LVM_SUCCESS;
}