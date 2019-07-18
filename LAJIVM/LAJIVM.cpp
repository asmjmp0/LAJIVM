#include<iostream>
#include<sstream>
#include"init.h"
#include"ErrorList.h"
#include"bin_read.h"
#include"ExecuteInstruction.h"
int main(int argc, char **argv) {
Begin:
	std::string str;
	std::string r_str;
	try { init_all(); }
	catch (int) {
		printf("初始化失败\n");
		goto End;
	}
	if (argc == 2) {
		r_str = argv[1];
		goto DOIT;
	}else if(argc == 3)
	{
		std::string temp = argv[1];
		if (temp == "d") {
			registe_ptr->R6 = 0;
			r_str = argv[2];
			goto DOIT;
		}else{
			printf("命令行参数错误\n");
			exit(1);
		}
	}
	printf("虚拟机初始化完成请输入二进制文件路径,输入ctrl+z退出：\n");
	if (!std::getline(std::cin,str)) exit(0);
	if (*str.begin() == 'd') {
		auto temp = str.find(' ', 1);
		if (temp != std::string::npos) {
			r_str = str.substr(temp + 1);
			registe_ptr->R6 = 0;
		}else r_str = str;
	}else r_str = str;
	DOIT:
	try { write_all(r_str); }
	catch (int){
		printf("打开二进制文件失败\n");
		goto End;
	}
	printf("执行二进制文件>>>>>>>>>>\n");
	try{
		while (registe_ptr->IP<=m_code_length) exectue_ins();
		goto Begin;
	}
	catch (int)
	{
		printf("运行过程中出错\n");
		goto End;
	}
End:
	return 0;
}