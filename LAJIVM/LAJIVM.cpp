#include<iostream>
#include"init.h"
#include"ErrorList.h"
#include"bin_read.h"
#include"ExecuteInstruction.h"
int main(int argc, char argv) {
Begin:
	std::string str;
	try { init_all(); }
	catch (int) {
		printf("初始化失败\n");
		goto End;
	}
	printf("初始化完成请输入二进制文件路径,输入ctrl+z退出：\n");
	if (!(std::cin >> str)) exit(0);
	try { write_all(); }
	catch (int){
		printf("打开二进制文件失败\n");
		goto End;
	}
	printf("***************************************************\n");
	printf("执行二进制文件>>>>>>>>>>\n\n");
	try{
		while (registe_ptr->IP<m_code_length) exectue_ins();
		goto Begin;
	}
	catch (int)
	{
		printf("运行过程中出错\n");
		goto End;
	}
	End: system("pause");
}