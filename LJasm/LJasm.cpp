#include"LJasm.h"
#include"AnalyseCode.h"
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<regex>
#include<ios>
std::vector<std::string>  origin_str{};
int bin_length{ 0 };
int asm_length{ 0 };
int now_index{ 0 };
int blank_row{ 0 };
int data_long{ 0 };
label_struct *label_s;
write_label_data *write_label_d;
int label_index{ 0 };
int write_label_index{ 0 };
uint8_t* out_ptr{ nullptr };
/*���Ժ���*/
void test_out_ptr(int num) {
	for (int i = 0; i < num; i++)
	{
		printf("%02X ", out_ptr[i]);
	}
}
/*
��16�����ַ���ת��������
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
			if (temp_str != "" && temp_str[0] != ';') {//�ж��Ƿ��ǿ��л���ע��
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
		std::cout << "�Ҳ���DATAͷ" << std::endl;
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
		std::cout << "�Ҳ���DATAβ" << std::endl;
		return (0 - now_index);
	}
	for (int i = 1; i < end_flag; i++)//��ȡ���ݶ�ÿ�е�����
	{
		for (int index = 0; index < origin_str[i].length();++index) {//�������ݶ�ÿ�е�ÿ���ַ�
			{
				if (origin_str[i][index] == '\\') {
					if (origin_str[i][index + 1] == '\\') {//ת���ַ�
						out_ptr[bin_length] = '\\';
						++bin_length;
						++index;
					}
					else if (origin_str[i][index + 1] == 'x')//16������
					{

						int num = string_to_hex(origin_str[i].substr(index + 2, 2));
						if ((unsigned)num == -1) {//ת�����󱨴�
							std::cout << "���ݶΰ����޷��������﷨" << std::endl;
							return (0 - now_index);
						}
						else {//д��ת���������
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
		++now_index;//��ǰִ����������
	}
	data_long = bin_length + 5;
	return 0;
}
int read_code() {
	int end_flag{ -1 };
	*(long long*)(out_ptr + bin_length) = 0x3a45444f43;//д��Ӳ����CODE:
	bin_length += 5;//bin_length����
	++now_index;
	if (origin_str[now_index] != "CODE:") {
		std::cout << "�Ҳ���CODEͷ" << std::endl;
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
		std::cout << "�Ҳ���CODEβ" << std::endl;
		return (0 - now_index);
	}
	++now_index;//ָ���һ�д���
	for (; now_index < end_flag; now_index++)
	{
		try
		{
			Analyse_code(origin_str[now_index]);
		}
		catch (int exception)
		{
			return (0 - now_index);
		}
	}
	return 0;
}
bool write_end_out(std::string str) {
	long long bin_end_code = 0x010101010101;//bin end
	*(long long *)(out_ptr+bin_length) = bin_end_code;
	bin_length += 6;
	std::regex reg("(.*\\\\)?(.*)\\..*$");//��ȡ�ļ�����
	std::smatch match;
	if (std::regex_match(str, match, reg)) {
		std::ofstream out_file(match[2], std::ios::out | std::ios::binary);
		out_file.write((char*)out_ptr, bin_length);
		out_file.close();
		return true;
	}
	else return false;
}
int main(int argc, char **argv) {
	char *path{nullptr};
	int wrong_data{0};
	int wrong_code{ 0 };
	if (argc > 1) path = argv[1];
	else {
		std::cout << "�����������м����ļ�·��" << std::endl;
		exit(-1);
	}
	if (!read_file(path)){
		std::cout << "���ļ�ʧ��" << std::endl;
		exit(-1);
	}
	out_ptr = (uint8_t*)malloc(BIN_MAX_SIZE_ * 1024 * 1024);
	memset(out_ptr, 0, BIN_MAX_SIZE_ * 1024 * 1024);
	if (out_ptr == nullptr) {
		std::cout << "��������ڴ�ʧ��" << std::endl;
		exit(-1);
	}
	/*
	*д�� LAJICM\XffDATA:Ӳ����
	*/
	*(long long*)out_ptr = 0x44ff4d56494a414c;
	*(unsigned*)(out_ptr + 8) = 0x3a415441;
	bin_length = 12;
	
	label_s = new label_struct[100];

	write_label_d = new write_label_data[100];

	wrong_data = read_data();
	if (wrong_data<0) {
		std::cout << "��ȡ���ݶ�ʧ��\n" <<"�����"<<(blank_row+1-wrong_data)<<"��"<< std::endl;
		exit(-1);
	}
	wrong_code = read_code();
	if (wrong_code < 0) {
		std::cout << "��ȡ�����ʧ��\n" << "�����" << (blank_row + 1 - wrong_code) << "��" << std::endl;
		exit(-1);
	}
	write_all_to_jmp();
	if (!write_end_out(std::string(path))) {
		std::cout << "������ļ�ʧ��" << std::endl;
		exit(-1);
	}
	return 0;
}