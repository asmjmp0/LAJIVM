#include"LJasm.h"
#include"AnalyseCode.h"
#include<cctype>
int (*analyse_list[0xff])(std::string);
int Analyse_code(std::string str) {//mov R0L,0 ;123456789
	unsigned comment_index{ str.find(';') };
	str = str.substr(0, comment_index);//除去注释
	for (auto &c:str) c=toupper(c);//全部转大写
	unsigned blank_index{ str.find_first_of(' ') };
	std::string op_str = str.substr(0, blank_index);//获取指令字符串
	std::string ins_str = str.substr(++blank_index, str.length()-blank_index);//获取操作指令字符串
	if (op_str == "MOV") Analyse_mov(ins_str);
	else if(op_str == "ADD") Analyse_add(ins_str);
	else
	{
		std::cout << "未定义的指令类型" << std::endl;
		throw(now_index);
	}
	return 0;
}
int Analyse_mov(std::string str) {
	unsigned slip_index{ str.find(',') };
	std::string a_str{ str.substr(0,slip_index) };
	std::string b_str{ str.substr(++slip_index,str.length()-slip_index) };

	return 0;
}
int Analyse_add(std::string str) {
	return 0;
}