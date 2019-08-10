#include<iostream>
#include<sstream>
#include"ExecuteInstruction.h"
#include"init.h"
bool debug_flag{ false };
bool uasm_flag{ false };
/*
*����ѡ��
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
	registe_ptr->R6 = (unsigned)-1;//�����ϵ�ر�
	do_ins();
}
void lvm_debug_u() {
	uasm_flag = true;
	unsigned temp = registe_ptr->IP;//���浱ǰIP Ϊ�������׼��
	while (registe_ptr->IP != m_code_length)
	{
		printf("%08X   ", registe_ptr->IP);
		int ins_len;
		ins_len = get_ins_len(code_ptr[registe_ptr->IP], code_ptr[registe_ptr->IP + 1]);//��ȡָ��� Ϊ��ַָ�������׼��
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
	if (data_adr.substr(0, 2) == "0x")//16�����ַ���
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
			printf_s("%02X ", ptr[data_num + m]);
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
		printf("------------------------����ģʽ------------------------\n");
	}
	debug_flag = true;
	printf("IP--------->%08X\n", registe_ptr->IP);
FI:
	std::string str{ "" };
	std::cout << ">>>>";
YC:
	std::getline(std::cin, str);
	if (str.empty()) goto YC;
	if (str == "n")//����
	{
		lvm_debug_n(ins_len);
		return;
	}
	else if (str == "e") {//�˳�����
		lvm_debug_e();
		return;
	}
	else if (str == "c") {//���г���
		lvm_debug_c();
		return;
	}
	else if (str == "u") {
		lvm_debug_u();
		goto FI;

	}
	else if (str == "p")//��ӡ������Ϣ
	{
		lvm_debug_p();
		goto FI;//���ض���
	}
	else if (*str.begin() == 'b') {
		auto temp = str.find(' ', 1);
		if (temp != std::string::npos) {
			std::string break_adr = str.substr(temp + 1);
			std::stringstream ss;
			ss.str("");
			unsigned break_num{ 0 };
			if (break_adr.substr(0, 2) == "0x")//16�����ַ���
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
			printf("��������ȷ�ĸ�ʽ\n");
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
			printf("��������ȷ�ĸ�ʽ\n");
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
			printf("��������ȷ�ĸ�ʽ\n");
			goto FI;
		}
	}
	else {
		printf("��������ȷ��ָ��\n");
		goto FI;
	}

}