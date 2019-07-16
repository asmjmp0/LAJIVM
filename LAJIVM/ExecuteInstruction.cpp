#include"ExecuteInstruction.h"
#include"ErrorList.h"
#include"init.h"
#include<iostream>
int _ins_len(char c) {
	switch (c)
	{
	case '\x00': {
		return 2;
	}
	case '\x01': {
		return 5;
	}
	case '\x02': {
		return 2;
	}
	case '\x03': {
		return 2;
	}
	case '\x04': {
		return 2;
	}
	}
	return -100;
}
/*
*获取指令长度
*/
int get_ins_len(char f, char s) {
	switch (f)
	{
	case INS_MOV: {
		if (s <= '\x04')
		return	2+_ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_LEA: {
		if (s <= '\x04')
			return	2 + _ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_INT: {
		//
		return 2;
		break;
	}
    default: {
		break;
	}
		
	}
	return -1;
}
/*
*执行mov指令
*/
int do_mov() {
	switch (code_ptr[registe_ptr->IP+1])
	{
	case '\x00': {
		//
		break;
	}
	case '\x01': {//
		if (code_ptr[registe_ptr->IP + 2] % 0x10 == 0)//低位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			char *R0 = (char *)&registe_ptr->R0;
			R0[0] = temp;
		}
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
*执行lea指令
*/
int do_lea() {
	switch (code_ptr[registe_ptr->IP + 1])
	{
	case '\x00': {
		//
		break;
	}
	case '\x01': {//
		if (code_ptr[registe_ptr->IP + 2] % 0x10 == 2)//全运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			registe_ptr->R3 = temp;
		}
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
*执行int指令
*/
int do_int() {
	switch ((char)registe_ptr->R0){
	case LVM_PRINT: {
		//
		printf("%s", data_ptr + registe_ptr->R3);
		break;
	}
	case LVM_EXIT: {
		printf("\n\n虚拟机成功退出\n\n");
	}
	}
	return LVM_SUCCESS;
}
/*
*指令执行选择器
*/
int do_ins(){
	switch (code_ptr[registe_ptr->IP])
	{
	case INS_MOV: {
		do_mov();
		break;
	}
	case INS_LEA: {
		do_lea();
		break;
	}
	case INS_INT: {
		do_int();
		break;
	}
	default: {
		throw(LVM_EXECUTE_ERROR);
		break;
	}
	}
	return LVM_SUCCESS;
}
int exectue_ins() {
	int ins_len;
	ins_len=get_ins_len(code_ptr[registe_ptr->IP], code_ptr[registe_ptr->IP + 1]);//获取指令长度 为地址指针递增做准备
	if(ins_len>0)
	do_ins();
	else throw(LVM_EXECUTE_ERROR);
	registe_ptr->IP += ins_len;
	return LVM_SUCCESS;
}