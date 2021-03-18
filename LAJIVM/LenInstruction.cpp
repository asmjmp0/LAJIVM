#include"headers/LenInstruction.h"
#include"headers/ErrorList.h"
#include<iostream>
#include"headers/init.h"
int _ins_len(char c) {
	uint8_t high = (uint8_t)c / 0x10;//获取操作类型高位
	uint8_t low = (uint8_t)c % 0x10;
	if (!uasm_flag) {
		if (high == 0) {//操纵数据段
			registe_ptr->flag[14] = 0;
		}
		else if (high == 1) {//操纵栈段
			registe_ptr->flag[14] = 1;
		}
		else if (high == 2);
		else throw(LVM_EXECUTE_ERROR);

	}
	switch (low)
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
	case '\x05': {
		return 1;
	}
	case '\x06': {
		return 4;
	}
	case '\x07': {
		return 5;
	}
	case '\x08': {
		return 5;
	}
	case '\x09': {
		return 3;
	}
	case '\x0a': {
		return 3;
	}
	}
	throw(LVM_EXECUTE_ERROR);
}
/*
*获取指令长度
*/
int get_ins_len(char f, char s) {
	uint8_t f1 = (uint8_t)f;
	switch (f1)
	{
	case INS_MOV: {
		if (s <= '\x44')
			return	2 + _ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
	}
	case INS_LEA: {
		if (s <= '\x44')
			return	2 + _ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
	}
	case INS_INT: {
		//
		return 2;
	}
	case INS_JMP:case INS_JZ:case INS_JNZ:case INS_JH:case INS_JL: {
		return 2 + _ins_len(s);
	}
	case INS_INC:case INS_DEC: {
		return 2;
	}
	case INS_CMP: {
		if (s <= '\x42') return	2 + _ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
	}
	case INS_ADD: case INS_SUB:case INS_XOR:case INS_AND:case INS_OR: {
		if (s <= '\x42')
		{
			return	2 + _ins_len(s);
		}
	}
	case INS_NOT: {
		return 2;
	}
	case INS_NOP: {
		return 1;
	}
	case INS_PUSH: {
		return 2 + _ins_len(s);
	}
	case INS_POP: {
		return 2;
	}
	case INS_CALL: {
		return 2 + _ins_len(s);
	}
	case INS_RET: {
		return 2 + _ins_len(s);
	}
	case INS_JIT_IN: {
		return 1;
	}
	default: {
		break;
	}

	}
	throw(LVM_EXECUTE_ERROR);
}