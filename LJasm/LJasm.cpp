#include"LJasm.h"
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
std::vector<std::string>  origin_str{};
int bin_length{ 0 };
int asm_length{ 0 };
int now_index{ 0 };
int blank_row{ 0 };
uint8_t* out_ptr{ nullptr };
/*
把16进制字符串转换成数字
*/
int string_to_hex(std::string str) {
	int data_num{ -1 };
	std::stringstream ss;
	ss << std::hex << str;
	ss >> data_num;
	return data_num;
}
bool read_file(char *path) {
	std::ifstream in;
	std::string temp_str{""};
	bool flag{ false };
	in.open(path, std::ifstream::in);
	if (!in.is_open()) return flag;
	else{
		while (std::getline(in, temp_str)) {
			++asm_length;
			if (temp_str != "" && temp_str[0] != ';') {//判断是否是空行或者注释
				origin_str.push_back(temp_str);
			}else ++blank_row;
		}
			
	}
	return !flag;

}
int read_data(){
	int end_flag{ -1 };
	++now_index;
	if (origin_str[0] != "DATA:") {
		std::cout << "找不到DATA头" << std::endl;
		return (0 - now_index);
	}
	for (int i = 1; i < origin_str.size();i++)
	{
		if (origin_str[i] == "DATA END;"){
			end_flag = i;
			break;
		}

	}
	if (end_flag == -1) {
		std::cout << "找不到DATA尾" << std::endl;
		return (0 - now_index);
	}
	for (int i = 1; i < end_flag; i++)//获取数据段每行的内容
	{
		for (int index = 0; index < origin_str[i].length();++index) {//分析数据段每行的每个字符
			{
				if (origin_str[i][index] == '\\') {
					if (origin_str[i][index + 1] == '\\') {//转义字符
						out_ptr[bin_length] = '\\';
						++bin_length;
						++index;
					}
					else if (origin_str[i][index + 1] == 'x')//16进制数
					{

						int num = string_to_hex(origin_str[i].substr(index + 2, 2));
						if ((unsigned)num == -1) {//转换错误报错
							std::cout << "数据段包含无法解析的语法" << std::endl;
							return (0 - now_index);
						}
						else {//写入转换后的数字
							uint8_t w_code = (uint8_t) num;
							out_ptr[bin_length] = w_code;
							index += 3;
							++bin_length;
						}
							
					}
				}
				else
				{
					out_ptr[bin_length] = origin_str[i][index];
					++bin_length;
				}
			}
		}
		++now_index;//当前执行行数递增
	}
	return 0;
}
int read_code() {
	int end_flag{ -1 };
	*(long long*)(out_ptr + bin_length) = 0x3a45444f43;//写入硬编码CODE:
	bin_length += 5;//bin_length递增
	++now_index;
	if (origin_str[now_index] != "CODE:") {
		std::cout << "找不到CODE头" << std::endl;
		return (0 - now_index);
	}
	for (int i = now_index+1; i < origin_str.size(); i++)
	{
		if (origin_str[i] == "CODE END;") {
			end_flag = i;
			break;
		}
	}
	if (end_flag == -1) {
		std::cout << "找不到DATA尾" << std::endl;
		return (0 - now_index);
	}
	for (int i = now_index; i < end_flag; i++)
	{
	std:;
	}
	return 0;
}
int main(int argc, char **argv) {
	char *path{nullptr};
	int wrong_data{0};
	int wrong_code{ 0 };
	if (argc > 1) path = argv[1];
	else {
		std::cout << "请在命令行中加入文件路径" << std::endl;
		exit(-1);
	}
	if (!read_file(path)){
		std::cout << "打开文件失败" << std::endl;
		exit(-1);
	}
	out_ptr = (uint8_t*)malloc(BIN_MAX_SIZE_ * 1024 * 1024);
	memset(out_ptr, 0, BIN_MAX_SIZE_ * 1024 * 1024);
	if (out_ptr == nullptr) {
		std::cout << "分配输出内存失败" << std::endl;
		exit(-1);
	}
	/*
	*写入 LAJICM\XffDATA:硬编码
	*/
	*(long long*)out_ptr = 0x44ff4d56494a414c;
	*(unsigned*)(out_ptr + 8) = 0x3a415441;
	bin_length = 12;
	
	wrong_data = read_data();
	if (wrong_data<0) {
		std::cout << "读取数据段失败\n" <<"错误第"<<(blank_row+1-wrong_data)<<"行"<< std::endl;
		exit(-1);
	}
	wrong_code = read_code();
	if (wrong_code < 0) {
		std::cout << "读取代码段失败\n" << "错误第" << (blank_row + 1 - wrong_code) << "行" << std::endl;
		exit(-1);
	}
	printf("%s\n", out_ptr);
	return 0;
}