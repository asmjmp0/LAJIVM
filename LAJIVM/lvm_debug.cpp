#include<iostream>
#include<sstream>
#include"headers/ExecuteInstruction.h"
#include"headers/ErrorList.h"
#include"headers/LenInstruction.h"
#include"headers/init.h"
bool debug_flag{ false };
bool uasm_flag{ false };
void lvm_debug_u_print(char a, char b);
/*
*调试选项
*/
void lvm_debug_n(int ins_len) {
	registe_ptr->R6 = registe_ptr->IP;
	registe_ptr->R6 += ins_len;
	registe_ptr->flag[15] = 1;
	do_ins();
}
void lvm_debug_e() {
	debug_flag = false;
	registe_ptr->R6 = (unsigned)-1;
	registe_ptr->R7 = (unsigned)-1;
	registe_ptr->flag[15] = 0;
	do_ins();
}
void lvm_debug_c() {
	registe_ptr->flag[15] = 0;
	registe_ptr->R6 = (unsigned)-1;//单步断点关闭

	do_ins();
}
void lvm_debug_u() {
	uasm_flag = true;
	unsigned temp = registe_ptr->IP;//保存当前IP 为反汇编做准备
	while (registe_ptr->IP != m_code_length+registe_ptr->CS)
	{
		printf("%08X   ", registe_ptr->IP);
		int ins_len;
		ins_len = get_ins_len(code_ptr[registe_ptr->IP], code_ptr[registe_ptr->IP + 1]);//获取指令长度 为地址指针递增做准备
		lvm_debug_u_print(code_ptr[registe_ptr->IP], code_ptr[registe_ptr->IP + 1]);
		registe_ptr->IP += ins_len;
	}
	registe_ptr->IP = temp;
	uasm_flag = false;
}
void lvm_debug_p() {
	int count{ 0 };
	for (int i = 0; i < 8; i++)
	{
		++count;
		std::cout << "R" << i << "--------->";
		printf("%08X	", *register_list[i]);
		if (count == 3) {
			count = 0;
			printf("\n");
		}
	}
	printf("IP--------->%08X\n", registe_ptr->IP);
	printf("SP--------->%08X	", registe_ptr->SP);
	printf("BP--------->%08X\n", registe_ptr->BP);
	printf("CS--------->%08X\n", registe_ptr->CS);
	printf("DS--------->%08X\n", registe_ptr->DS);
	printf("flag[0]--------->%02x	", registe_ptr->flag[0]);
	printf("flag[1]--------->%02x	", registe_ptr->flag[1]);
	printf("flag[14]--------->%02x\n", registe_ptr->flag[14]);
}
void lvm_debug_d_s(char * ptr,std::string str,size_t temp) {
	if (ptr == data_ptr)
		printf("DATA:\n");
	else if (ptr == stack_ptr)
		printf("STACK:SP--->%08X\n", registe_ptr->SP);
	std::string data_adr = str.substr(temp + 1);
	std::stringstream ss;
	ss.str("");
	unsigned data_num{ 0 };
	if (data_adr.substr(0, 2) == "0x")//16进制字符串
	{
		ss << std::hex << data_adr.substr(2);
		ss >> data_num;
	}
	else
	{
		ss << data_adr;
		ss >> data_num;
	}
	int m{ 0 };
	for (int i = 0; i < 3; i++)
	{
		printf("%08X	", data_num + m);
		for (int h = 0; h< 16; h++)
		{
			printf("%02X ", (unsigned char *)ptr[data_num + m]);
			++m;
		}
		int p = m - 16;
		putchar('|');
		for (int o = p; o< m; o++)
		{
			if (ptr[data_num + o] == '\x0a' || ptr[data_num + o] == '\x0d') putchar('*');
			else putchar(ptr[data_num + o]);
		}
		printf("\n");
	}
}
void lvm_debug(int ins_len) {

	if (debug_flag != true) {
		printf("------------------------调试模式------------------------\n");
	}
	debug_flag = true;
	printf("IP--------->%08X\n", registe_ptr->IP);
FI:
	std::string str{ "" };
	std::cout << ">>>>";
YC:
	std::getline(std::cin, str);
	if (str.empty()) goto YC;
	if (str == "n")//单步
	{
		lvm_debug_n(ins_len);
		return;
	}
	else if (str == "e") {//退出调试
		lvm_debug_e();
		return;
	}
	else if (str == "c") {//运行程序
		lvm_debug_c();
		return;
	}
	else if (str == "u") {
		lvm_debug_u();
		goto FI;

	}
	else if (str == "p")//打印调试信息
	{
		lvm_debug_p();
		goto FI;//返回顶端
	}
	else if (*str.begin() == 'b') {
		auto temp = str.find(' ', 1);
		if (temp != std::string::npos) {
			std::string break_adr = str.substr(temp + 1);
			std::stringstream ss;
			ss.str("");
			unsigned break_num{ 0 };
			if (break_adr.substr(0, 2) == "0x")//16进制字符串
			{
				ss << std::hex << break_adr.substr(2);
				ss >> break_num;
			}
			else
			{
				ss << break_adr;
				ss >> break_num;
			}
			registe_ptr->R7 = break_num;
			printf("R7--------->%08X\n", registe_ptr->R7);
			goto FI;
		}
		else
		{
			printf("请输入正确的格式\n");
			goto FI;
		}
	}
	else if (str[0] == 'd' && str[1] == 'b')
	{
		auto temp = str.find(' ', 2);
		if (temp != std::string::npos) {
			lvm_debug_d_s(data_ptr, str, temp);
			goto FI;
		}
		else
		{
			printf("请输入正确的格式\n");
			goto FI;
		}
	}else if(str[0] == 's' && str[1] == 'b')
	{
		auto temp = str.find(' ', 2);
		if (temp != std::string::npos) {
			lvm_debug_d_s(stack_ptr,str,temp);
			goto FI;
		}
		else
		{
			printf("请输入正确的格式\n");
			goto FI;
		}
	}
	else {
		printf("请输入正确的指令\n");
		goto FI;
	}

}
void lvm_debug_u_print(char a, char b) {
	switch ((unsigned char)a)
	{
	case INS_PUSH: {
		printf("push ");
		break;
	}
	case INS_MOV: {
		printf("mov ");
		break;
	}
	case INS_LEA: {
		printf("lea ");
	}
	case INS_INT: {
		printf("int i\n");
		return;
		break;
	}
	case INS_JMP:case INS_JZ:case INS_JNZ:case INS_JH:case INS_JL: {
			if (a == INS_JMP)
				printf("jmp ");
			else if (a == INS_JZ)
				printf("jz ");
			else if (a == INS_JNZ)
				printf("jnz ");
			else if (a == INS_JH)
				printf("jh ");
			else if (a == INS_JH)
				printf("jl ");
		break;
	}
	case INS_INC:case INS_DEC: {
			if (a == INS_INC)
				printf("inc R\n");
			else if (a == INS_DEC)
				printf("dec R\n ");
		break;
	}
	case INS_CMP: {
		printf("cmp ");
		break;
	}
	case INS_ADD: case INS_SUB:case INS_XOR:case INS_AND:case INS_OR: {
		if (a == INS_ADD)
			printf("add ");
		else if (a == INS_SUB)
			printf("sub ");
		else if (a == INS_XOR)
			printf("xor ");
		else if (a == INS_AND)
			printf("and ");
		else if (a == INS_OR)
			printf("or ");
		break;
	}
	case INS_NOT: {
		printf("not R\n");
		break;
	}
	case INS_NOP: {
		printf("nop\n");
		return;
		break;
	}
	case INS_JIT_IN: {
		printf("jit\n");
		return;
		break;
	}
	case INS_POP: {
		printf("pop R\n");
		return;
	}
	case INS_CALL: {
		printf("call ");
		break;
	}
	case INS_RET: {
		printf("ret ");
		break;
	}
	default: {
		throw(LVM_EXECUTE_ERROR);
	}
	}
	uint8_t high = (uint8_t)b / 0x10;//获取操作类型高位
	uint8_t low = (uint8_t)b % 0x10;
	switch (low)
	{
	case '\x00': {
		printf("R,R\n");
		break;
	}
	case '\x01': {
		printf("R,i\n");
		break;
	}
	case '\x02': {
		if (high == 1) {
			printf("R,s[R]\n");
		}
		else {
			printf("R,[R]\n");
		}
		break;
	}
	case '\x03': {
		if (high == 1)
			printf("s[R],R\n");
		else {
			printf("[R],R\n");
		}
		break;
	}
	case '\x04': {
		printf("[R],[R]\n");
		break;
	}
	case '\x05': {
		printf("R\n");
		break;
	}
	case '\x06': {
		printf("i\n");
		break;
	}
	case '\x07': {
		if (high == 1)
			printf("R,s[R+i]\n");
		else {
			printf("R,[R+i]\n");
		}
		break;
	}
	case '\x08': {
		if (high == 1)
			printf("s[R+i],R\n");
		else {
			printf("[R+i],R\n");
		}
		break;
	}
	case '\x09': {
		if (high == 1)
			printf("s[R+R],R\n");
		else {
			printf("[R+R],R\n");
		}
		break;
	}
	case '\x0a': {
		if (high == 1)
			printf("R,s[R+R]\n");
		else {
			printf("R,[R+R]\n");
		}
		break;
	}
	default:
		throw(LVM_EXECUTE_ERROR);
	}
}