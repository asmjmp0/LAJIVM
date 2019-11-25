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
		std::cout << "δ�ҵ�JIT��ʼ��" << std::endl;
		exit(-1);
	}
	jit_end = now_index;
	std::cout << "JIT����" << std::endl;
	jit_str_init();
	jit_create_jit_file();
	return 0;
}
void jit_create_jit_file() {
	/*TODO:����asm.exe���ɶ������ļ�����ȡshellcode*/
	/*���ɻ���ļ�*/
	std::regex reg("(.*\\\\)?(.*)\\..*$");//��ȡ�ļ�����
	std::smatch match;
	char *shcode_origin{nullptr};
	std::regex_match(file_name, match, reg);
	
	std::ofstream out_file(match[2].str()+".asm", std::ios::out | std::ios::binary);
	out_file.write(asm_str.c_str(),asm_str.length());
	out_file.close();
	/*�������ļ�*/
	std::string cmd = +"asm.exe /c ";
	cmd += match[2].str() + ".asm";
	system(cmd.c_str());
	/*��ȡ����ļ�*/
	std::ifstream in;
	in.open(match[2].str()+".obj", std::ifstream::in);
	//����ʧ��
	if (!in.is_open()) {
		std::cout << "JIT���ɻ�����ʧ��" << std::endl;
		exit(-1);
	}
	else {
		/*��ȡ������*/
		int info_size = 16;
		in.seekg(0, std::ios::end);//�Ƶ��ļ�β��
		int shcode_size = in.tellg();//��ȡ�ļ���С
		shcode_origin = new char[shcode_size];
		in.seekg(0, std::ios::beg);//�Ƶ��ļ�ͷ��
		in.read(shcode_origin, shcode_size);
		in.close();
		unsigned shcode_len = *(unsigned*)(shcode_origin + 0x24);
		unsigned shcode_index = *(unsigned*)(shcode_origin + 0x28);
		char *jit_out = new char[info_size + shcode_len];
		/*����jit�ļ�*/
		jit_out[0] = 'J';
		jit_out[1] = 'I';
		jit_out[2] = 'T';
		*(unsigned*)(jit_out + 3) = shcode_len;
		*(unsigned*)(jit_out + 7) = jit_index;
		jit_out[11] = jit_type;
		*(unsigned*)(jit_out + 12) = jit_bin_end;
		memcpy(jit_out + info_size, shcode_origin + shcode_index, shcode_len);
		/*д���ļ�*/
		std::ofstream out(match[2].str()+".jit", std::ios::out | std::ios::binary);
		if (out.is_open()) {
			out.write(jit_out, (shcode_len + info_size));
			out.close();
		}
		/*ɾ����ʱ�ļ�*/
		remove((match[2].str() + ".asm").c_str());
		remove((match[2].str() + ".obj").c_str());
	}
}
void jit_macro_change(std::string &str) {
	/*�滻��������Ϊx86ԭ�����*/
	//����Ƿ���� r6,r7
	if (str.find("RET") != std::string::npos) jit_type = 1;
	if (str.find("R6") != std::string::npos || str.find("R7") != std::string::npos) {
		std::cout << "JIT��֧�ֶ�R6,R7����" << std::endl;
		exit(-1);
	}
	int change_count = 0;
	//�滻ȫ����
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
	/*��ȡ��Ҫjit����Ĵ���
	���Һ��滻�Ĵ�������*/
	for (int i = jit_begin+1; i < jit_end; i++)
	{
		std::string str= origin_str[i];
		unsigned comment_index{ str.find(';') };
		 str = str.substr(0, comment_index);//��ȥע��
		 for (auto &c : str) c = toupper(c);//ȫ��ת��д
		/*���滻*/
		 jit_macro_change(str);
		asm_str += str+'\n';
	}
	if (jit_type == 0) asm_str += "ret\n";
	asm_str += "end";
}