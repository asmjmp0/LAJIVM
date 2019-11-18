#include"init.h"
#include"ErrorList.h"
#include"ExecuteInstruction.h"
#include<iostream>
int do_push() {
	registe_ptr->SP += 4;//Õ»Ö¸ÕëÔö¼Ó
	if (code_ptr[registe_ptr->IP + 1] == 5) {//¼Ä´æÆ÷Ñ¹Õ»
		uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
		unsigned num = *register_list[high];
		*(unsigned*)(stack_ptr + registe_ptr->SP) = num;
	}
	else if (code_ptr[registe_ptr->IP + 1] == 6) {//Á¢¼´ÊýÑ¹Õ»
		unsigned num = *(unsigned*)(code_ptr + registe_ptr->IP + 2);
		*(unsigned*)(stack_ptr + registe_ptr->SP) = num;
	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
int do_pop() {
	
	unsigned num = *(unsigned*)(stack_ptr + registe_ptr->SP);//»ñÈ¡Õ»¶¥Öµ
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 1] / 0x10;//»ñÈ¡¼Ä´æ±àºÅ
	*register_list[high] = num;//¼Ä´æÆ÷¸³Öµ

	registe_ptr->SP -= 4;
	return LVM_SUCCESS;
}