#include"LJasm.h"
#include"JITcomp.h"
#include"AnalyseCode.h"
#include<regex>
#include<fstream>
#include<ios>
#include<Windows.h>


int jit_begin=0;
int jit_end=0;
unsigned jit_bin_end = 0;
char jit_type;
std::string asm_str = ".386\n.model flat\n.code\n";
const static std::string register_vm_name_list[]={ "R0","R1","R2","R3","R4","R5","SP","BP" };
const static std::string register_x86_name_list[]{ "eax","ebx","ecx","edx","esi","edi","esp","ebp" };
const static std::string register_x86_high_name_list[]{ "ax","bx","cx","dx" };
const static std::string register_x86_low_name_list[]{ "al","bl","cl","dl" };
int jit_index = 0;
int Analyse_jit_in() {
	jit_index = bin_length-data_long;
	out_ptr[bin_length] = INS_JIT_IN;
	++bin_length;
	jit_begin = now_index;
	return 0;
}
int Analyse_jit_out() {
	jit_bin_end = bin_length - data_long;
	if (jit_begin == 0) {
		std::cout << "未找到JIT开始点" << std::endl;
		exit(-1);
	}
	jit_end = now_index;
	std::cout << "JIT编译" << std::endl;
	jit_str_init();
	jit_create_jit_file();
	return 0;
}
void jit_create_jit_file() {
	/*TODO:利用asm.exe生成二进制文件并获取shellcode*/
	/*生成汇编文件*/
	std::regex reg("(.*\\\\)?(.*)\\..*$");//获取文件名字
	std::smatch match;
	char *shcode_origin{nullptr};
	std::regex_match(file_name, match, reg);
	
	std::ofstream out_file(match[2].str()+".asm", std::ios::out | std::ios::binary);
	out_file.write(asm_str.c_str(),asm_str.length());
	out_file.close();
	/*编译汇编文件*/
	std::string cmd = +"asm.exe /c ";
	cmd += match[2].str() + ".asm";
	system(cmd.c_str());
	/*读取汇编文件*/
	std::ifstream in;
	in.open(match[2].str()+".obj", std::ifstream::in);
	//编译失败
	if (!in.is_open()) {
		std::cout << "JIT生成机器码失败" << std::endl;
		exit(-1);
	}
	else {
		/*获取机器码*/
		int info_size = 16;
		in.seekg(0, std::ios::end);//移到文件尾部
		int shcode_size = in.tellg();//获取文件大小
		shcode_origin = new char[shcode_size];
		in.seekg(0, std::ios::beg);//移到文件头部
		in.read(shcode_origin, shcode_size);
		in.close();
		unsigned shcode_len = *(unsigned*)(shcode_origin + 0x24);
		unsigned shcode_index = *(unsigned*)(shcode_origin + 0x28);
		char *jit_out = new char[info_size + shcode_len];
		/*构造jit文件*/
		jit_out[0] = 'J';
		jit_out[1] = 'I';
		jit_out[2] = 'T';
		*(unsigned*)(jit_out + 3) = shcode_len;
		*(unsigned*)(jit_out + 7) = jit_index;
		jit_out[11] = jit_type;
		*(unsigned*)(jit_out + 12) = jit_bin_end;
		memcpy(jit_out + info_size, shcode_origin + shcode_index, shcode_len);
		/*写入文件*/
		std::ofstream out(match[2].str()+".jit", std::ios::out | std::ios::binary);
		if (out.is_open()) {
			out.write(jit_out, (shcode_len + info_size));
			out.close();
		}
		/*删除临时文件*/
		remove((match[2].str() + ".asm").c_str());
		remove((match[2].str() + ".obj").c_str());
	}
}
void jit_macro_change(std::string &str) {
	/*替换虚拟机汇编为x86原生汇编*/
	//检查是否存在 r6,r7
	if (str.find("RET") != std::string::npos) jit_type = 1;
	if (str.find("R6") != std::string::npos || str.find("R7") != std::string::npos) {
		std::cout << "JIT不支持对R6,R7操作" << std::endl;
		exit(-1);
	}
	int change_count = 0;
	//替换全类型
	size_t r_index = std::string::npos;

	for (int i = 0; i < 6; i++)
	{
		r_index = str.find(register_vm_name_list[i] + "H");
		if (r_index != std::string::npos) {
			str = str.replace(r_index, 3, register_x86_high_name_list[i]);
			++change_count;
			r_index = str.find(register_vm_name_list[i]);
			if (r_index != std::string::npos) {
				str = str.replace(r_index, 3, register_x86_high_name_list[i]);
				++change_count;
			}
		}
	}
	if (change_count == 3) return;
	for (int i = 0; i < 6; i++)
	{
		r_index = str.find(register_vm_name_list[i] + "L");
		if (r_index != std::string::npos) {
			str = str.replace(r_index, 3, register_x86_low_name_list[i]);
			++change_count;
			r_index = str.find(register_vm_name_list[i]);
			if (r_index != std::string::npos) {
				str = str.replace(r_index, 3, register_x86_low_name_list[i]);
				++change_count;
			}
		}
	}
	if (change_count == 3) return;
	int len = 8;
	for (int i = 0; i < len; i++)
	{
		r_index = str.find(register_vm_name_list[i]);
		if (r_index != std::string::npos) {
			str=str.replace(r_index, 2, register_x86_name_list[i]);
			++change_count;
			r_index = str.find(register_vm_name_list[i]);
			if (r_index != std::string::npos) {
				str = str.replace(r_index, 2, register_x86_name_list[i]);
				++change_count;
			}
		}
	}

}
void jit_str_init(){
	/*获取需要jit编译的代码
	并且宏替换寄存器名称*/
	for (int i = jit_begin+1; i < jit_end; i++)
	{
		std::string str= origin_str[i];
		unsigned comment_index{ str.find(';') };
		 str = str.substr(0, comment_index);//除去注释
		 for (auto &c : str) c = toupper(c);//全部转大写
		/*宏替换*/
		 jit_macro_change(str);
		asm_str += str+'\n';
	}
	if (jit_type == 0) asm_str += "ret\n";
	asm_str += "end";
}