#include"init.h"
#include"ErrorList.h"
#include"ExecuteInstruction.h"
#include<iostream>
int do_push() {
	registe_ptr->SP += 4;//ջָ������
	if (code_ptr[registe_ptr->IP + 1] == 5) {//�Ĵ���ѹջ
		uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
		unsigned num = *register_list[high];
		*(unsigned*)(stack_ptr + registe_ptr->SP) = num;
	}
	else if (code_ptr[registe_ptr->IP + 1] == 6) {//������ѹջ
		unsigned num = *(unsigned*)(code_ptr + registe_ptr->IP + 2);
		*(unsigned*)(stack_ptr + registe_ptr->SP) = num;
	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
int do_pop() {
	
	unsigned num = *(unsigned*)(stack_ptr + registe_ptr->SP);//��ȡջ��ֵ
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 1] / 0x10;//��ȡ�Ĵ���
	*register_list[high] = num;//�Ĵ�����ֵ

	registe_ptr->SP -= 4;
	return LVM_SUCCESS;
}