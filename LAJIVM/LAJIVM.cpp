#include<iostream>
#include<sstream>
#include"init.h"
#include"ErrorList.h"
#include"bin_read.h"
#include"ExecuteInstruction.h"
bool is_debug = false;
int main(int argc, char **argv) {
Begin:
	std::string str;
	std::string r_str;
	try { init_all(); }
	catch (int e) {
		printf("��ʼ��ʧ�� %08X\n",e);
		goto End;
	}
	if (argc == 2) {
		r_str = argv[1];
		goto DOIT;
	}else if(argc == 3)
	{
		std::string temp = argv[1];
		if (temp == "d") {
			is_debug = true;
			r_str = argv[2];
			goto DOIT;
		}else{
			printf("�����в�������\n");
			exit(1);
		}
	}
	int path_len = strlen(argv[0]);
	int stack_offset{ 0 };

	printf("�������ʼ�����������������ļ�·��,����ctrl+z�˳���\n");
	if (!std::getline(std::cin,str)) exit(0);
	if (*str.begin() == 'd') {
		auto temp = str.find(' ', 1);
		if (temp != std::string::npos) {
			r_str = str.substr(temp + 1);
			is_debug = true;
		}
		else r_str = str;
	}
	else r_str = str;
	DOIT:
	try { write_all(r_str); }
	catch (int e){
		printf("�򿪶������ļ�ʧ�� %08X\n",e);
		goto End;
	}
	printf("ִ�ж������ļ�>>>>>>>>>>\n");
	try{
		if(is_debug)registe_ptr->R6 = registe_ptr->CS;
		registe_ptr->IP = registe_ptr->IP + registe_ptr->CS;
		while (registe_ptr->IP<=m_code_length+registe_ptr->CS) 
			exectue_ins();
		goto Begin;
	}
	catch (int e)
	{
		printf("���й����г��� %08X\n",e);
		goto End;
	}
End:
	return 0;
}