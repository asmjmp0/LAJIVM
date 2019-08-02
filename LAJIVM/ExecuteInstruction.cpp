#include"ExecuteInstruction.h"
#include"ErrorList.h"
#include"init.h"
#include<iostream>
#include<string>
#include<ios>
#include<sstream>
/*
*��λ��ʾ�Ĵ�������
*��λ��ʾ�����Ĵ����ĳ���
*/
int _ins_len(char c) {
	uint8_t high = (uint8_t)c / 0x10;//��ȡ�������͸�λ
	uint8_t low = (uint8_t)c % 0x10;
	if (!uasm_flag) {
		if (high == 0) {//�������ݶ�
			registe_ptr->flag[14] = 0;
		}
		else if (high == 1) {//����ջ��
			registe_ptr->flag[14] = 1;
		}
		else if (high == 2);
		else throw(LVM_EXECUTE_ERROR);

	}
	switch (low)
	{
	case '\x00': {
		if (uasm_flag)printf("R,R\n");
		return 2;
	}
	case '\x01': {
		if (uasm_flag)printf("R,i\n");
		return 5;
	}
	case '\x02': {
		if (uasm_flag) {
			if (high == 1) {
				printf("R,s[R]\n");
			}
			else {
				printf("R,[R]\n");
			}
		}
		return 2;
	}
	case '\x03': {
		if (uasm_flag) {
			if (high == 1)
				printf("s[R],R\n");
			else {
				printf("[R],R\n");
			}
		}
		return 2;
	}
	case '\x04': {
		if (uasm_flag)printf("[R],[R]\n");
		return 2;
	}
	case '\x05': {
		if (uasm_flag)printf("R\n");
		return 1;
	}
	case '\x06': {
		if (uasm_flag)printf("i\n");
		return 4;
	}
	case '\x07': {
		if (uasm_flag)printf("R,[R+i]\n");
		return 5;
	}
	case '\x08': {
		if (uasm_flag)printf("[R+i],R\n");
		return 5;
	}
	case '\x09': {
		if (uasm_flag) {
			if (high == 1)
				printf("s[R+R],R\n");
			else {
				printf("[R+R],R\n");
			}
		}
			
		return 3;
	}
	case '\x0a': {
		if (uasm_flag) {
			if (high == 1)
				printf("R,s[R+R]\n");
			else {
				printf("R,[R+R]\n");
			}
		}

		return 3;
	}
	}
	throw(LVM_EXECUTE_ERROR);
	return -1;
}
/*
*��ȡָ���
*/
int get_ins_len(char f, char s) {
	uint8_t f1 = (uint8_t)f;
	switch (f1)
	{
	case INS_MOV: {
		if (uasm_flag)printf("50 mov ");
		if (s <= '\x44')
		return	2+_ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_LEA: {
		if (uasm_flag)printf("51 lea ");
		if (s <= '\x44')
			return	2 + _ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_INT: {
		//
		if (uasm_flag)printf("E0 int i\n");
		return 2;
		break;
	}
	case INS_JMP:case INS_JZ:case INS_JNZ:case INS_JH:case INS_JL: {
		if (uasm_flag) {
			if (f == INS_JMP) 
				printf("40 jmp ");
			else if(f == INS_JZ)
				printf("41 jz ");
			else if (f == INS_JNZ)
				printf("42 jnz ");
			else if (f == INS_JH)
				printf("43 jh ");
			else if (f == INS_JH)
				printf("44 jl ");
		}
		return 2+_ins_len(s);
		break;
	}
	case INS_INC:case INS_DEC: {
		if (uasm_flag) {
			if (f == INS_INC)
				printf("30 inc R\n");
			else if (f == INS_DEC)
				printf("31 dec R\n ");
		}
		return 2;
		break;
	}
	case INS_CMP: {
		if (s <= '\x42')
		{
			if (uasm_flag) printf("32 cmp ");
			return	2 + _ins_len(s);
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_ADD: case INS_SUB:case INS_XOR:case INS_AND:case INS_OR: {
		if (s <= '\x42')
		{
			if (uasm_flag) {
				if (f == INS_ADD)
					printf("20 add ");
				else if (f == INS_SUB)
					printf("21 sub ");
				else if (f == INS_XOR)
					printf("22 xor ");
				else if (f == INS_AND)
					printf("23 and ");
				else if (f == INS_OR)
					printf("24 or ");
			}
			return	2 + _ins_len(s);
		}
	}
	case INS_NOT: {
		if (uasm_flag) printf("25 not R\n");
		return 2;
	}
	case INS_NOP: {
		if (uasm_flag) printf("5F nop\n");
		return 1;
	}
	case INS_PUSH: {
		if (uasm_flag) {
			printf("10 push ");
		}
		return 2 + _ins_len(s);
	}
	case INS_POP: {
		if (uasm_flag) {
			printf("11 pop R\n");
		}
		return 2;
	}
    default: {
		break;
	}
		
	}
	throw(LVM_EXECUTE_ERROR);
	return -1;
}
/*
*���ñ�־�Ĵ��� ��������
*/
int set_flag(char pram) {
	char * ptr = registe_ptr->flag;//��ȡflagָ��
	if(pram== 0){
		ptr[0] = 0;
		return LVM_SUCCESS;
	}
	else if (pram > 0) {
		ptr[0] = 1;
		ptr[1] = 1;
		return LVM_SUCCESS;
	}else if(pram<0)
	{
		ptr[0] = 1;
		ptr[1] = 0;
		return LVM_SUCCESS;
	}
	return LVM_FAILD;
}
int set_flag(int16_t pram) {
	char * ptr = registe_ptr->flag;//��ȡflagָ��
	if (pram == 0) {
		ptr[0] = 0;
		return LVM_SUCCESS;
	}
	else if (pram > 0) {
		ptr[0] = 1;
		ptr[1] = 1;
		return LVM_SUCCESS;
	}
	else if (pram<0)
	{
		ptr[0] = 1;
		ptr[1] = 0;
		return LVM_SUCCESS;
	}
	return LVM_FAILD;
}
int set_flag(unsigned pram) {
	int temp = (int) pram;
	char * ptr = registe_ptr->flag;//��ȡflagָ��
	if (temp == 0) {//�����������0λ��0
		ptr[0] = 0;
		return LVM_SUCCESS;
	}
	else if (temp > 0) {//������0 0λ��1
		ptr[0] = 1;
		ptr[1] = 1;
		return LVM_SUCCESS;

	}
	else if (temp<0)
	{
		ptr[0] = 1;
		ptr[1] = 0;
		return LVM_SUCCESS;
	}
	return LVM_FAILD;
}
/*
*ִ��movָ��
*/
int do_mov() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low= (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;

	uint8_t o_low = (uint8_t)code_ptr[registe_ptr->IP + 1] % 0x10;

	switch (o_low)//��������
	{
	case '\x00': {//mov R,R  ---->0
		//
		uint8_t highb = code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra = *Rb;
			set_flag(*Ra);
			break;
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra = *Rb;
			set_flag(*Ra);
			break;
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra = *Rb;
			set_flag(*Ra);
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case '\x01': {//mov R,i   ----->1
		if (low == 0)//��λ����
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
			char *R = (char *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(short*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			unsigned *R = (unsigned *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case '\x02': {//R,[R] ----->2
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		char *data_temp_p = data_ptr;
		if (registe_ptr->flag[14] == 1) data_ptr = stack_ptr;//�ٿ�ջ��
		if (low == 0)//��λ����
		{
			unsigned value = *(unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			char temp = data_ptr[value];//��ȡ���ݶε�����
			char *R = (char *)register_list[high];
			*R = temp;
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else if (low == 1)//��λ����
		{
			unsigned value = *(unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			int16_t temp = *(int16_t*)(data_ptr + value);//��ȡ���ݶε�����
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else if (low == 2)//ȫ����
		{
			unsigned value = *(unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			unsigned temp = *(unsigned*)(data_ptr + value);//��ȡ���ݶε�����
			unsigned *R = (unsigned *)register_list[high];
			*R = temp;
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case '\x03': {//[R],R----->3
		uint8_t lowb = (uint8_t)code_ptr[registe_ptr->IP + 3] % 0x10;
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		char *data_temp_p = data_ptr;
		if (registe_ptr->flag[14] == 1) data_ptr = stack_ptr;//�ٿ�ջ��
		if (lowb == 0)//��λ����
		{
			unsigned value = *(unsigned*)register_list[high];//���Ra�Ĵ����е�ֵ
			char *temp = data_ptr + value;//��ȡ����ָ��
			char *R = (char *)register_list[highb];//���Rb�Ĵ����е�ֵ
			*temp = *R;//д��
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else if (lowb == 1)//��λ����
		{
			unsigned value = *(unsigned*)register_list[high];//���Ra�Ĵ����е�ֵ
			int16_t *temp = (int16_t*)(data_ptr + value);//��ȡ����ָ��
			int16_t *R = (int16_t*)register_list[highb];//���Rb�Ĵ����е�ֵ
			*temp = *R;//д��
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else if (lowb == 2)//ȫ����
		{
			unsigned value = *(unsigned*)register_list[high];//���Ra�Ĵ����е�ֵ
			unsigned *temp = (unsigned*)(data_ptr + value);//��ȡ����ָ��
			unsigned *R = (unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			*temp = *R;//д��
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case '\x07': {//R, [R + i]----->07
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		short i = *(short*)(code_ptr + registe_ptr->IP + 5);//��ȡ������

		char *data_temp_p = data_ptr;
		if (registe_ptr->flag[14] == 1) data_ptr = stack_ptr;//�ٿ�ջ��
		if (low == 0)//��λ����
		{
			unsigned value = *(unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			char temp = data_ptr[i + value];//��ȡ���ݶε�����
			char *R = (char *)register_list[high];
			*R = temp;
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else if (low == 1)//��λ����
		{
			unsigned value = *(unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			int16_t temp = *(int16_t*)(data_ptr + i + value);//��ȡ���ݶε�����
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else if (low == 2)//ȫ����
		{
			unsigned value = *(unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			unsigned temp = *(unsigned*)(data_ptr + i + value);//��ȡ���ݶε�����
			unsigned *R = (unsigned *)register_list[high];
			*R = temp;
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case '\x08': {//[R+i],R----->08
		uint8_t lowb = (uint8_t)code_ptr[registe_ptr->IP + 6] % 0x10;
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 6] / 0x10;
		short i = *(short*)(code_ptr + registe_ptr->IP + 4);//��ȡ������

		char *data_temp_p = data_ptr;
		if (registe_ptr->flag[14] == 1) data_ptr = stack_ptr;//�ٿ�ջ��
		if (lowb == 0)//��λ����
		{
			unsigned value = *(unsigned*)register_list[high];//���Ra�Ĵ����е�ֵ
			char *temp = data_ptr + i + value;//��ȡ����ָ��
			char *R = (char *)register_list[highb];//���Rb�Ĵ����е�ֵ
			*temp = *R;//д��
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else if (lowb == 1)//��λ����
		{
			unsigned value = *(unsigned*)register_list[high];//���Ra�Ĵ����е�ֵ
			int16_t *temp = (int16_t*)(data_ptr + i + value);//��ȡ����ָ��
			int16_t *R = (int16_t*)register_list[highb];//���Rb�Ĵ����е�ֵ
			*temp = *R;//д��
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else if (lowb == 2)//ȫ����
		{
			unsigned value = *(unsigned*)register_list[high];//���Ra�Ĵ����е�ֵ
			unsigned *temp = (unsigned*)(data_ptr + i + value);//��ȡ����ָ��
			unsigned *R = (unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			*temp = *R;//д��
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		break;
	}
	case'\x09': {//mov [R+R],R----->09
		uint8_t lowb1 = (uint8_t)code_ptr[registe_ptr->IP + 3] % 0x10;
		uint8_t highb1 = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		uint8_t lowb = (uint8_t)code_ptr[registe_ptr->IP + 4] % 0x10;
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 4] / 0x10;
		char *data_temp_p = data_ptr;
		if (registe_ptr->flag[14] == 1) data_ptr = stack_ptr;//�ٿ�ջ��
		if (lowb == 0)//��λ����
		{
			unsigned value = *(unsigned*)register_list[high];//���Ra�Ĵ����е�ֵ
			unsigned value1 = *(unsigned*)register_list[highb1];//���Ra1�Ĵ����е�ֵ
			char *temp = data_ptr + value + value1;//��ȡ����ָ��
			char *R = (char *)register_list[highb];//���Rb�Ĵ����е�ֵ
			*temp = *R;//д��
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else if (lowb == 1)//��λ����
		{
			unsigned value = *(unsigned*)register_list[high];//���Ra�Ĵ����е�ֵ
			unsigned value1 = *(unsigned*)register_list[highb1];//���Ra1�Ĵ����е�ֵ
			int16_t *temp = (int16_t*)(data_ptr + value + value1);//��ȡ����ָ��
			int16_t *R = (int16_t*)register_list[highb];//���Rb�Ĵ����е�ֵ
			*temp = *R;//д��
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else if (lowb == 2)//ȫ����
		{
			unsigned value = *(unsigned*)register_list[high];//���Ra�Ĵ����е�ֵ
			unsigned value1 = *(unsigned*)register_list[highb1];//���Ra1�Ĵ����е�ֵ
			unsigned *temp = (unsigned*)(data_ptr + value + value1);//��ȡ����ָ��
			unsigned *R = (unsigned*)register_list[highb];//���Rb�Ĵ����е�ֵ
			*temp = *R;//д��
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case '\x0a': {//mov R,[R+R]----->0a
		uint8_t lowb1 = (uint8_t)code_ptr[registe_ptr->IP + 3] % 0x10;
		uint8_t highb1 = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		uint8_t lowb = (uint8_t)code_ptr[registe_ptr->IP + 4] % 0x10;
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 4] / 0x10;
		char *data_temp_p = data_ptr;
		if (registe_ptr->flag[14] == 1) data_ptr = stack_ptr;//�ٿ�ջ��
		if (low == 0)//��λ����
		{
			unsigned value = *(unsigned*)register_list[highb];//���Ra�Ĵ����е�ֵ
			unsigned value1 = *(unsigned*)register_list[highb1];//���Ra1�Ĵ����е�ֵ
			char *temp = data_ptr + value + value1;//��ȡ����ָ��
			char *R = (char *)register_list[high];//���Rb�Ĵ����е�ֵ
			*R = *temp;//д��
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else if (low == 1)//��λ����
		{
			unsigned value = *(unsigned*)register_list[highb];//���Ra�Ĵ����е�ֵ
			unsigned value1 = *(unsigned*)register_list[highb1];//���Ra1�Ĵ����е�ֵ
			int16_t *temp = (int16_t*)(data_ptr + value + value1);//��ȡ����ָ��
			int16_t *R = (int16_t*)register_list[high];//���Rb�Ĵ����е�ֵ
			*R = *temp;//д��
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}
		else if (low == 2)//ȫ����
		{
			unsigned value = *(unsigned*)register_list[highb];//���Ra�Ĵ����е�ֵ
			unsigned value1 = *(unsigned*)register_list[highb1];//���Ra1�Ĵ����е�ֵ
			unsigned *temp = (unsigned*)(data_ptr + value + value1);//��ȡ����ָ��
			unsigned *R = (unsigned*)register_list[high];//���Rb�Ĵ����е�ֵ
			*R = *temp;//д��
			if (registe_ptr->flag[14] == 1) data_ptr = data_temp_p;//��ԭ����
			break;
		}

	}
	default: {
		throw(LVM_EXECUTE_ERROR);
		break;
	}
	}
	return LVM_SUCCESS;

}
/*
*ִ��leaָ��
*/
int do_lea() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	switch (code_ptr[registe_ptr->IP + 1])//��������
	{
	case '\x00': {
		//
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra = *Rb;
			break;
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra = *Rb;
			break;
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra = *Rb;
			break;
		}
		break;
	}
	case '\x01': {//
		if (low == 0)//��λ����
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//���������
			char *R = (char *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(short*)(code_ptr+registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr+registe_ptr->IP + 2 + 1);//���������
			unsigned *R = (unsigned *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	default: {
		throw(LVM_EXECUTE_ERROR);
		break;
	}
	}
	return LVM_SUCCESS;
}
/*
*ִ��intָ��
*/
int do_int() {
	switch ((char)registe_ptr->R0){
	case LVM_PRINT: {
		int temp{ 0 };
		temp=printf_s("%s", data_ptr + registe_ptr->R3);
		registe_ptr->R0 = temp;
		break;
	}
	case LVM_EXIT: {
		exit(0);
	}
	case LVM_SCAN: {
		int temp{ 0 };
		temp=scanf_s("%s", data_ptr + registe_ptr->R3,registe_ptr->R4);
		registe_ptr->R0 = temp;
		break;
	}
	}
	return LVM_SUCCESS;
}
/*
*ָ��ִ��ѡ����
*/
int do_ins(){
	if(ins_list[(uint8_t)code_ptr[registe_ptr->IP]]!=nullptr)
	ins_list[(uint8_t)code_ptr[registe_ptr->IP]]();
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*ִ��JMP
*/
int do_jmp() {
	if (code_ptr[registe_ptr->IP + 1] == 5) {//�Ĵ�����ת
		uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
		unsigned adr = *register_list[high];
		registe_ptr->IP = adr;
	}else if (code_ptr[registe_ptr->IP + 1] == 6){//��������ת
		unsigned adr = *(unsigned*)(code_ptr+registe_ptr->IP + 2);
		registe_ptr->IP = adr;
		
	}else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*ִ��JZ
*/
int do_jz() {
	if (registe_ptr->flag[0] == 0){
		if (code_ptr[registe_ptr->IP + 1] == 5) {//�Ĵ�����ת
			uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
			unsigned adr = *register_list[high];
			registe_ptr->IP = adr;
		}
		else if (code_ptr[registe_ptr->IP + 1] == 6) {//��������ת
			unsigned adr = *(unsigned*)(code_ptr + registe_ptr->IP + 2);
			registe_ptr->IP = adr;

		}
		else throw(LVM_EXECUTE_ERROR);
	}
	return LVM_SUCCESS;
}
/*
*ִ��JNZ
*/
int do_jnz() {
	if (registe_ptr->flag[0] == 1) {//�жϷ�����ת����
		if (code_ptr[registe_ptr->IP + 1] == 5) {//�Ĵ�����ת
			uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
			unsigned adr = *register_list[high];
			registe_ptr->IP = adr;
		}
		else if (code_ptr[registe_ptr->IP + 1] == 6) {//��������ת
			unsigned adr = *(unsigned*)(code_ptr+registe_ptr->IP + 2);
			registe_ptr->IP = adr;

		}else throw(LVM_EXECUTE_ERROR);
	}
	return LVM_SUCCESS;
}
/*
*ִ��JH
*/
int do_jh() {
	if (registe_ptr->flag[1] == 1) {
		if (code_ptr[registe_ptr->IP + 1] == 5) {//�Ĵ�����ת
			uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
			unsigned adr = *register_list[high];
			registe_ptr->IP = adr;
		}
		else if (code_ptr[registe_ptr->IP + 1] == 6) {//��������ת
			unsigned adr = *(unsigned*)(code_ptr + registe_ptr->IP + 2);
			registe_ptr->IP = adr;

		}
		else throw(LVM_EXECUTE_ERROR);
	}
	return LVM_SUCCESS;
}
/*
*ִ��JL
*/
int do_jl() {
	if (registe_ptr->flag[1] == 0) {
		if (code_ptr[registe_ptr->IP + 1] == 5) {//�Ĵ�����ת
			uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
			unsigned adr = *register_list[high];
			registe_ptr->IP = adr;
		}
		else if (code_ptr[registe_ptr->IP + 1] == 6) {//��������ת
			unsigned adr = *(unsigned*)(code_ptr + registe_ptr->IP + 2);
			registe_ptr->IP = adr;

		}
		else throw(LVM_EXECUTE_ERROR);
	}
	return LVM_SUCCESS;
}
/*
*ִ��INC
*/
int do_inc(){
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 1] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 1] % 0x10;
	switch (low)
	{
	case 0: {
		char*Ra = (char *)register_list[high];
		++*Ra;
		set_flag(*Ra);
		break;
	}
	case 1: {
		int16_t *Ra = (int16_t *)register_list[high];
		++*Ra;
		set_flag(*Ra);
		break;
	}
	case 2: {
		unsigned *Ra = (unsigned *)register_list[high];
		++*Ra;
		set_flag(*Ra);
		break;
	}
	default:
	{
		throw(LVM_EXECUTE_ERROR);
		break;
	}
	}
	return LVM_SUCCESS;
}
/*
*ִ��DEC
*/
int do_dec() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 1] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 1] % 0x10;
	switch (low)
	{
	case 0: {
		char*Ra = (char *)register_list[high];
		--*Ra;
		set_flag(*Ra);
		break;
	}
	case 1: {
		int16_t *Ra = (int16_t *)register_list[high];
		--*Ra;
		set_flag(*Ra);
		break;
	}
	case 2: {
		unsigned *Ra = (unsigned *)register_list[high];
		--*Ra;
		set_flag(*Ra);
		break;
	}
	default:
	{
		throw(LVM_EXECUTE_ERROR);
		break;
	}
	}
	return LVM_SUCCESS;
}
/*
*ִ��CMP
*/
int do_cmp() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == '\x00') {//�Ĵ����Ƚ�
			uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
			if (low == 0) {
				char*Ra = (char *)register_list[high];
				char*Rb = (char*)register_list[highb];
				char temp = *Ra - *Rb;
				set_flag(temp);
			}
			else if (low == 1)
			{
				int16_t*Ra = (int16_t *)register_list[high];
				int16_t*Rb = (int16_t *)register_list[highb];
				int16_t temp = *Ra - *Rb;
				set_flag(temp);
			}
			else if (low == 2)
			{
				unsigned*Ra = (unsigned *)register_list[high];
				unsigned*Rb = (unsigned*)register_list[highb];
				unsigned temp = *Ra - *Rb;
				set_flag(temp);
			}
			else throw(LVM_EXECUTE_ERROR);
	}
	else if (code_ptr[registe_ptr->IP + 1] == '\x01') {//�������Ƚ�
		unsigned i = code_ptr[registe_ptr->IP + 3];
		set_flag(*register_list[high] - i);
	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*ִ��ADD
*/
int do_add() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1]==0)//0������
	{
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra += *Rb;
			set_flag(*Ra);
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra += *Rb;
			set_flag(*Ra);
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra += *Rb;
			set_flag(*Ra);
		}else throw(LVM_EXECUTE_ERROR);
	}
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1������
		if (low == 0)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr+registe_ptr->IP + 2 + 1);//���������
			char *R = (char *)register_list[high];
			*R += temp;
			set_flag(*R);
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R += temp;
			set_flag(*R);
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			unsigned *R = (unsigned *)register_list[high];
			*R += temp;
			set_flag(*R);
		}
		else throw(LVM_EXECUTE_ERROR);

	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*ִ��SUB
*/
int do_sub() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0������
	{
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra -= *Rb;
			set_flag(*Ra);
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra -= *Rb;
			set_flag(*Ra);
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra -= *Rb;
			set_flag(*Ra);
		}
		else throw(LVM_EXECUTE_ERROR);
	}
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1������
		if (low == 0)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			char *R = (char *)register_list[high];
			*R -= temp;
			set_flag(*R);
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R -= temp;
			set_flag(*R);
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			unsigned *R = (unsigned *)register_list[high];
			*R -= temp;
			set_flag(*R);
		}
		else throw(LVM_EXECUTE_ERROR);

	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*ִ��XOR
*/
int do_xor() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0������
	{
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra ^= *Rb;
			set_flag(*Ra);
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra ^= *Rb;
			set_flag(*Ra);
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra ^= *Rb;
			set_flag(*Ra);
		}
		else throw(LVM_EXECUTE_ERROR);
	}
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1������
		if (low == 0)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			char *R = (char *)register_list[high];
			*R ^= temp;
			set_flag(*R);
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R ^= temp;
			set_flag(*R);
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			unsigned *R = (unsigned *)register_list[high];
			*R ^= temp;
			set_flag(*R);
		}
		else throw(LVM_EXECUTE_ERROR);

	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*ִ��AND
*/
int do_and() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0������
	{
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra &= *Rb;
			set_flag(*Ra);
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra &= *Rb;
			set_flag(*Ra);
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra &= *Rb;
			set_flag(*Ra);
		}
		else throw(LVM_EXECUTE_ERROR);
	}
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1������
		if (low == 0)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			char *R = (char *)register_list[high];
			*R &= temp;
			set_flag(*R);
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R &= temp;
			set_flag(*R);
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			unsigned *R = (unsigned *)register_list[high];
			*R &= temp;
			set_flag(*R);
		}
		else throw(LVM_EXECUTE_ERROR);

	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*ִ��OR
*/
int do_or() {
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 2] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0������
	{
		uint8_t highb = (uint8_t)code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra = (char *)register_list[high];
			char*Rb = (char*)register_list[highb];
			*Ra |= *Rb;
			set_flag(*Ra);
		}
		else if (low == 1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra |= *Rb;
			set_flag(*Ra);
		}
		else if (low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra |= *Rb;
			set_flag(*Ra);
		}
		else throw(LVM_EXECUTE_ERROR);
	}
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1������
		if (low == 0)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			char *R = (char *)register_list[high];
			*R |= temp;
			set_flag(*R);
		}
		else if (low == 1)//��λ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			int16_t *R = (int16_t *)register_list[high];
			*R |= temp;
			set_flag(*R);
		}
		else if (low == 2)//ȫ����
		{
			unsigned temp = *(unsigned*)(code_ptr + registe_ptr->IP + 2 + 1);//���������
			unsigned *R = (unsigned *)register_list[high];
			*R |= temp;
			set_flag(*R);
		}
		else throw(LVM_EXECUTE_ERROR);

	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*ִ��NOT
*/
int do_not(){
	uint8_t high = (uint8_t)code_ptr[registe_ptr->IP + 1] / 0x10;
	uint8_t low = (uint8_t)code_ptr[registe_ptr->IP + 1] % 0x10;
	if (low == 0) {
		char*Ra = (char *)register_list[high];
		*Ra = ~*Ra;
		set_flag(*Ra);
	}
	else if (low == 1)
	{
		int16_t*Ra = (int16_t *)register_list[high];
		*Ra = ~*Ra;
		set_flag(*Ra);
	}
	else if (low == 2)
	{
		unsigned*Ra = (unsigned *)register_list[high];
		*Ra = ~*Ra;
		set_flag(*Ra);
	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*ִ��NOP
*/
int do_nop() {
	return LVM_SUCCESS;
}
int exectue_ins() {
	int ins_len;
	ins_len=get_ins_len(code_ptr[registe_ptr->IP], code_ptr[registe_ptr->IP + 1]);//��ȡָ��� Ϊ��ַָ�������׼��
	unsigned tempIP = registe_ptr->IP;
	if (ins_len > 0)
	{
		if (registe_ptr->R6 == registe_ptr->IP || registe_ptr->R7 == registe_ptr->IP)//�������״̬
		{
			lvm_debug(ins_len);
		}
		else do_ins();
	}
	else throw(LVM_EXECUTE_ERROR);
	if (tempIP == registe_ptr->IP)//����Ƿ�����ת
	{
		registe_ptr->IP += ins_len;
	}
	else
	{
		if (debug_flag && registe_ptr->flag[15]) registe_ptr->R6 = registe_ptr->IP;
	}
		
	
	return LVM_SUCCESS;
}