#include"bin_read.h"
#include"ErrorList.h"
#include"BinType.h"
#include"init.h"
#include<fstream>
#include<ios>
#include<iostream>
using std::ifstream;
using std::ios;
char origin_bin[0xffff];
int index{0};
int file_size{ 0 };
unsigned m_code_length;
/*
*比较字节是否相同
*/
bool bincmp(char * a, char *b, int len) {
	bool flag{ true };
	for (int i = 0; i < len; i++)
	{
		flag =(a[i] == b[i]);
		if (!flag) return flag;
	}
	return flag;
}
/*
*查找字符串返回长度 失败返回-1
*因为没有时间 所以采用垃圾算法
*/
int find_str(char* a, char* b,int lena,int lenb) {
	for (int i = 0; i < lena - lenb; i++)
	{
		if (bincmp(a + i, b, lenb))
			return i;
	}

	return -1;
}
/*
*读取二进制文件
*/
int read_bin(std::string str) {
	ifstream bin(str, ios::in | ios::binary);
	if (bin) {
		std::cout << "打开文件成功！" << std::endl;
		bin.seekg(0, ios::end);//移到文件尾部
		file_size = bin.tellg();//获取文件大小
		bin.seekg(0, ios::beg);//移到文件头部
		bin.read(origin_bin, file_size);
		int stack_offset;
		memcpy(stack_ptr + registe_ptr->SP, str.c_str(), str.length() + 1);//先写再加
		if (str.length() + 1 > 4) {
			if ((str.length() + 1) % 4 == 0) {//实际路径为取得长度+\x00
				stack_offset = str.length() +1;
				registe_ptr->SP += stack_offset;
			}
		else
		{
			stack_offset = 4 * ((str.length()+1) / 4) + 4;
			registe_ptr->SP += stack_offset;
		}
	}
	else
	{
		registe_ptr->SP += 4;
	}
	bin.close();
	return LVM_SUCCESS;
	}else throw(LVM_BIN_OPEN_ERROR);
}
/*
*把二进制文件中的数据段写入内存
*/
int write_to_data() {
	int data_len;
	if (!bincmp(origin_bin+index,LVM_SIGNE,BIN_HEAD_LEN))
	{
		std::cout << "不是这个虚拟机的可执行二进制文件" << std::endl;
		throw(LVM_BIN_READ_ERROR);
	}else index = BIN_HEAD_LEN;
	if (!bincmp(origin_bin+index,LVM_DATA,BIN_DATA_LEN))
	{
		std::cout << "不是这个虚拟机的可执行二进制文件" << std::endl;
		throw(LVM_BIN_READ_ERROR);
	}else index += BIN_DATA_LEN;
	data_len = find_str(origin_bin + index, LVM_CODE, file_size - index, BIN_CODE_LEN);
	if (data_len == -1){
		std::cout << "不是这个虚拟机的可执行二进制文件" << std::endl;
		throw(LVM_BIN_READ_ERROR);
	}else
	{
		memcpy(data_ptr, origin_bin + index, data_len);
		//std::cout << "复制到数据段成功！" << std::endl;
	}
	index += data_len;
	return LVM_SUCCESS;
}
/*
*把二进制文件中的代码段写入内存
*/
int write_to_code(){
	int code_len;
	index += BIN_CODE_LEN;//下标指向代码段数据
	code_len = find_str(origin_bin + index, LVM_END, file_size - index, BIN_END_LEN);
	m_code_length = (unsigned)code_len;
	if (code_len == -1) {
		std::cout << "不是这个虚拟机的可执行二进制文件" << std::endl;
		throw(LVM_BIN_READ_ERROR);
	}else
	{
		memcpy(code_ptr, origin_bin + index, code_len);
		//std::cout << "复制到代码段成功！" << std::endl;
	}
	return LVM_SUCCESS;
}
void write_all(std::string str) {
	read_bin(str);
	write_to_data();
	write_to_code();
	index = 0;
}