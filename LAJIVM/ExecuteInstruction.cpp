#include"ExecuteInstruction.h"
#include"ErrorList.h"
#include"init.h"
#include<iostream>
#include<string>
#include<ios>
#include<sstream>
bool debug_flag{ false };
bool uasm_flag{ false };
/*
*高位表示寄存器类型
*地位表示操作寄存器的长度
*/
int _ins_len(char c) {
	switch (c)
	{
	case '\x00': {
		if (uasm_flag )printf("R,R\n");
		return 2;
	}
	case '\x01': {
		if (uasm_flag)printf("R,i\n");
		return 5;
	}
	case '\x02': {
		if (uasm_flag)printf("R,[R]\n");
		return 2;
	}
	case '\x03': {
		if (uasm_flag)printf("[R],R\n");
		return 2;
	}
	case '\x04': {
		if (uasm_flag)printf("[R],[R]\n");
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
		if (uasm_flag)printf("mov ");
		if (s <= '\x04')
		return	2+_ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_LEA: {
		if (uasm_flag)printf("lea ");
		if (s <= '\x04')
			return	2 + _ins_len(s);
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_INT: {
		//
		if (uasm_flag)printf("int i\n");
		return 2;
		break;
	}
	case INS_JMP:case INS_JZ:case INS_JNZ:case INS_JH:case INS_JL: {
		if (uasm_flag) {
			if (f == INS_JMP) 
				printf("jmp i\n");
			else if(f == INS_JZ)
				printf("jz i\n");
			else if (f == INS_JNZ)
				printf("jnz i\n");
			else if (f == INS_JH)
				printf("jh i\n");
			else if (f == INS_JH)
				printf("jl i\n");
		}
		return 5;
		break;
	}
	case INS_INC:case INS_DEC: {
		if (uasm_flag) {
			if (f == INS_INC)
				printf("inc R\n");
			else if (f == INS_DEC)
				printf("dec R\n ");
		}
		return 2;
		break;
	}
	case INS_CMP: {
		if (s <= '\x02')
		{
			if (uasm_flag) printf("cmp ");
			return	2 + _ins_len(s);
		}
		else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case INS_ADD: case INS_SUB:case INS_XOR:case INS_AND:case INS_OR:{
		if (s <= '\x02')
		{
			if (uasm_flag) {
				if (f == INS_ADD)
					printf("add ");
				else if (f == INS_SUB)
					printf("sub ");
				else if (f == INS_XOR)
					printf("xor ");
				else if (f == INS_AND)
					printf("and ");
				else if (f == INS_OR)
					printf("or ");
			}
			return	2 + _ins_len(s);
		}
	case INS_NOT: {
		if (uasm_flag) printf("not R\n");
		return 2;
	}
	}
    default: {
		break;
	}
		
	}
	return -1;
}
/*
*设置标志寄存器 三次重载
*/
int set_flag(char pram) {
	char * ptr = registe_ptr->flag;//获取flag指针
	if(pram== 0){
		ptr[0] = 1;
		return LVM_SUCCESS;
	}
	else if (pram > 0) {
		ptr[0] = 0;
		ptr[1] = 1;
		return LVM_SUCCESS;
	}else if(pram<0)
	{
		ptr[0] = 0;
		ptr[1] = 0;
		return LVM_SUCCESS;
	}
	return LVM_FAILD;
}
int set_flag(int16_t pram) {
	char * ptr = registe_ptr->flag;//获取flag指针
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
	char * ptr = registe_ptr->flag;//获取flag指针
	if (temp == 0) {//操作结果等于0位置0
		ptr[0] = 0;
		return LVM_SUCCESS;
	}
	else if (temp > 0) {//不等于0 0位置1
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
*执行mov指令
*/
int do_mov() {
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
	int8_t low= code_ptr[registe_ptr->IP + 2] % 0x10;

	switch (code_ptr[registe_ptr->IP+1])//操作类型
	{
	case '\x00': {//mov R,R  ---->0
		//
		int8_t highb = code_ptr[registe_ptr->IP + 3] / 0x10;
		if (low == 0) {
			char*Ra= (char *)register_list[high];
			char*Rb=(char*)register_list[highb];
			*Ra = *Rb;
			set_flag(*Ra);
			break;
		}else if (low==1)
		{
			int16_t*Ra = (int16_t *)register_list[high];
			int16_t*Rb = (int16_t *)register_list[highb];
			*Ra = *Rb;
			set_flag(*Ra);
			break;
		}else if(low == 2)
		{
			unsigned*Ra = (unsigned *)register_list[high];
			unsigned*Rb = (unsigned*)register_list[highb];
			*Ra = *Rb;
			set_flag(*Ra);
			break;
		}else throw(LVM_EXECUTE_ERROR);
		break;
	}
	case '\x01': {//mov R,i   ----->1
		if (low == 0)//低位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			char *R = (char *)register_list[high];
			*R= temp;
			set_flag(*R);
			break;
		}else if (low==1)//高位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}else if(low ==2)//全运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
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
*执行lea指令
*/
int do_lea() {
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 2] % 0x10;
	switch (code_ptr[registe_ptr->IP + 1])//操作类型
	{
	case '\x00': {
		//
		int8_t highb = code_ptr[registe_ptr->IP + 3] / 0x10;
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
		if (low == 0)//低位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			char *R = (char *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else if (low == 1)//高位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R = temp;
			set_flag(*R);
			break;
		}
		else if (low == 2)//全运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
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
		printf("\n\n虚拟机成功退出");
		exit(0);
	}
	}
	return LVM_SUCCESS;
}
/*
*指令执行选择器
*/
int do_ins(){
	if(ins_list[code_ptr[registe_ptr->IP]]!=nullptr)
	ins_list[code_ptr[registe_ptr->IP]]();
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*执行JMP
*/
int do_jmp() {
	unsigned adr = code_ptr[registe_ptr->IP + 1];
	registe_ptr->IP = adr;
	return LVM_SUCCESS;
}
/*
*执行JZ
*/
int do_jz() {
	unsigned adr = code_ptr[registe_ptr->IP + 1];
	if(registe_ptr->flag[0]==0)registe_ptr->IP = adr;
	return LVM_SUCCESS;
}
/*
*执行JNZ
*/
int do_jnz() {
	unsigned adr = code_ptr[registe_ptr->IP + 1];
	if (registe_ptr->flag[0] == 1)registe_ptr->IP = adr;
	return LVM_SUCCESS;
}
/*
*执行JH
*/
int do_jh() {
	unsigned adr = code_ptr[registe_ptr->IP + 1];
	if (registe_ptr->flag[1] == 1)registe_ptr->IP = adr;
	return LVM_SUCCESS;
}
/*
*执行JL
*/
int do_jl() {
	unsigned adr = code_ptr[registe_ptr->IP + 1];
	if (registe_ptr->flag[1] == 0)registe_ptr->IP = adr;
	return LVM_SUCCESS;
}
/*
*执行INC
*/
int do_inc(){
	int8_t high = code_ptr[registe_ptr->IP + 1] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 1] % 0x10;
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
*执行DEC
*/
int do_dec() {
	int8_t high = code_ptr[registe_ptr->IP + 1] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 1] % 0x10;
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
*执行CMP
*/
int do_cmp() {
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
	if (code_ptr[registe_ptr->IP + 1] == '\x00') {
	}
	else if (code_ptr[registe_ptr->IP + 1] == '\x01') {
		unsigned i = code_ptr[registe_ptr->IP + 3];
		set_flag(*register_list[high] - i);
	}
	else throw(LVM_EXECUTE_ERROR);
	return LVM_SUCCESS;
}
/*
*执行ADD
*/
int do_add() {
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1]==0)//0型命令
	{
		int8_t highb = code_ptr[registe_ptr->IP + 3] / 0x10;
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
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1型命令
		if (low == 0)//低位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			char *R = (char *)register_list[high];
			*R += temp;
			set_flag(*R);
		}
		else if (low == 1)//高位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R += temp;
			set_flag(*R);
		}
		else if (low == 2)//全运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
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
*执行SUB
*/
int do_sub() {
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0型命令
	{
		int8_t highb = code_ptr[registe_ptr->IP + 3] / 0x10;
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
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1型命令
		if (low == 0)//低位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			char *R = (char *)register_list[high];
			*R -= temp;
			set_flag(*R);
		}
		else if (low == 1)//高位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R -= temp;
			set_flag(*R);
		}
		else if (low == 2)//全运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
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
*执行XOR
*/
int do_xor() {
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0型命令
	{
		int8_t highb = code_ptr[registe_ptr->IP + 3] / 0x10;
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
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1型命令
		if (low == 0)//低位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			char *R = (char *)register_list[high];
			*R ^= temp;
			set_flag(*R);
		}
		else if (low == 1)//高位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R ^= temp;
			set_flag(*R);
		}
		else if (low == 2)//全运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
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
*执行AND
*/
int do_and() {
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0型命令
	{
		int8_t highb = code_ptr[registe_ptr->IP + 3] / 0x10;
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
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1型命令
		if (low == 0)//低位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			char *R = (char *)register_list[high];
			*R &= temp;
			set_flag(*R);
		}
		else if (low == 1)//高位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R &= temp;
			set_flag(*R);
		}
		else if (low == 2)//全运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
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
*执行OR
*/
int do_or() {
	int8_t high = code_ptr[registe_ptr->IP + 2] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 2] % 0x10;
	if (code_ptr[registe_ptr->IP + 1] == 0)//0型命令
	{
		int8_t highb = code_ptr[registe_ptr->IP + 3] / 0x10;
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
	else if (code_ptr[registe_ptr->IP + 1] == 1) {//1型命令
		if (low == 0)//低位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			char *R = (char *)register_list[high];
			*R |= temp;
			set_flag(*R);
		}
		else if (low == 1)//高位运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
			int16_t *R = (int16_t *)register_list[high];
			*R |= temp;
			set_flag(*R);
		}
		else if (low == 2)//全运算
		{
			unsigned temp = code_ptr[registe_ptr->IP + 2 + 1];//获得立即数
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
*执行NOT
*/
int do_not(){
	int8_t high = code_ptr[registe_ptr->IP + 1] / 0x10;
	int8_t low = code_ptr[registe_ptr->IP + 1] % 0x10;
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
*调试选项
*/
void lvm_debug(int ins_len) {
	debug_flag = true;
	printf("----------------------调试模式----------------------\n");
	printf("IP--------->%08X\n", registe_ptr->IP);
FI:
	std::string str{""};
	std::cout << ">>>>";
YC:
	std::getline(std::cin, str);
	if (str.empty()) goto YC;
	if (str == "n")//单步
	{
		registe_ptr->R6 += ins_len;
		registe_ptr->flag[15] = 1;
		do_ins();
		printf("IP--------->%08X\n", registe_ptr->IP);
		return;
	}
	else if (str=="e") {//退出调试
		debug_flag = false;
		registe_ptr->R6 = (unsigned)-1;
		registe_ptr->flag[15] = 0;
		do_ins();
		return;
	}else if (str == "c") {//运行程序
		registe_ptr->flag[15] = 0;
		do_ins();
		return;
	}
	else if (str=="u") {
		uasm_flag = true;
		unsigned temp = registe_ptr->IP;//保存当前IP 为反汇编做准备
		while (registe_ptr->IP!=m_code_length)
		{
			printf("%08X   ", registe_ptr->IP);
			int ins_len;
			ins_len = get_ins_len(code_ptr[registe_ptr->IP], code_ptr[registe_ptr->IP + 1]);//获取指令长度 为地址指针递增做准备
			registe_ptr->IP += ins_len;
		}
		registe_ptr->IP = temp;
		uasm_flag = false;
		goto FI;

	}else if (str == "p")//打印调试信息
	{
		for (int i = 0; i < 7; i++)
		{
			std::cout << "R" << i << "--------->" ;
			printf("%08X\n", *register_list[i]);
		}
		printf("IP--------->%08X\n", registe_ptr->IP);
		printf("flag[0]--------->%02x\n", registe_ptr->flag[0]);
		printf("flag[1]--------->%02x\n", registe_ptr->flag[1]);
		goto FI;//返回顶端
	}if (*str.begin() == 'b') {
		auto temp=str.find(' ', 1);
		if (temp != std::string::npos) {
			std::string break_adr = str.substr(temp + 1);
			std::stringstream ss;
			ss.str("");
			unsigned break_num{ 0 };
			if (break_adr.substr(0, 2)=="0x")//16进制字符串
			{
				ss<< std::hex << break_adr.substr(2);
				ss >> break_num;
			}else
			{
				ss <<  break_adr;
				ss >> break_num;
			}
			registe_ptr->R6 = break_num;
			printf("R6--------->%08X\n", registe_ptr->R6);
			goto FI;
		}else
		{
			printf("请输入正确的格式\n");
			goto FI;
		}
	}
	else{
		printf("请输入正确的指令\n");
		goto FI;
	}

}
int exectue_ins() {
	int ins_len;
	ins_len=get_ins_len(code_ptr[registe_ptr->IP], code_ptr[registe_ptr->IP + 1]);//获取指令长度 为地址指针递增做准备
	unsigned tempIP = registe_ptr->IP;
	if (ins_len > 0)
	{
		if (registe_ptr->R6 == registe_ptr->IP)//陷入调试状态
		{
			lvm_debug(ins_len);
		}
		else do_ins();
	}
	else throw(LVM_EXECUTE_ERROR);
	if (tempIP == registe_ptr->IP)//检查是否发生跳转
	{
		registe_ptr->IP += ins_len;
	}
	else
	{
		if (debug_flag && registe_ptr->flag[15]) registe_ptr->R6 = registe_ptr->IP;
	}
		
	
	return LVM_SUCCESS;
}