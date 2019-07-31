#include"LJasm.h"
#include"AnalyseCode.h"
#include<cctype>
#include<sstream>
std::regex reg0("^R([0-9]{1,2})([LH]?)$");//匹配0 1 2 12 13 型式命令
std::regex reg1("^(0X)?([0-9]*)$");//匹配立即数
unsigned immed_to_unsgned(std::string is16, std::string str){
	if (is16 == "0X") {//为16进制数
		int data_num{ -1 };
		std::stringstream ss;
		ss << std::hex << str;
		ss >> data_num;
		return (unsigned)data_num;
	}
	else if (is16 == "") {//10进制数
		int data_num{ -1 };
		std::stringstream ss;
		ss << str;
		ss >> data_num;
		return (unsigned)data_num;
	}
}
uint8_t registernum_to_num(std::string str) {
	std::regex reg("([0-9]{1,2})[LH]?$");
	std::smatch match;
	if (std::regex_match(str, match, reg)) {
		return atoi(match.str().c_str());
	}
	else throw(now_index);
}
uint8_t lowhigh_to_num(std::string str) {
	if (str == "L") return 0;
	else if (str == "H") return 1;
	else if (str == "") return 2;
	else throw (now_index);
}
int Analyse_code(std::string str) {//mov R0L,0 ;123456789
	unsigned comment_index{ str.find(';') };
	str = str.substr(0, comment_index);//除去注释
	for (auto &c:str) c=toupper(c);//全部转大写
	unsigned blank_index{ str.find_first_of(' ') };
	std::string op_str = str.substr(0, blank_index);//获取指令字符串
	std::string ins_str = str.substr(++blank_index, str.length()-blank_index);//获取操作指令字符串
	if (op_str == "MOV") Analyse_mov(ins_str);
	else if(op_str == "ADD") Analyse_add(ins_str);
	else if (op_str == "NOP") {
		out_ptr[bin_length] = INS_NOP;
		++bin_length;
	}
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
	std::smatch amatch;
	std::smatch bmatch;
	bool f1=std::regex_match(a_str, amatch, reg0);
	if (f1) {//匹配0 1 2 12  型式命令成功
		bool ftemp = std::regex_match(b_str, bmatch, reg0);
		if (ftemp) {//匹配0型成功 mov R,R  ---->0
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			uint8_t b0 = registernum_to_num(bmatch[1]);
			uint8_t b1 = lowhigh_to_num(bmatch[2]);
			uint8_t b = b0 * 0x10 | b1;
			/*写入out_ptr*/
			out_ptr[bin_length] = INS_MOV;
			++bin_length;
			out_ptr[bin_length] = 0;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = b;
			++bin_length;
			++now_index;
			return 0;
		}else if(std::regex_match(b_str, bmatch, reg1))//匹配1型成功 mov R,i   ----->1
		{
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			unsigned i = immed_to_unsgned(bmatch[1],bmatch[2]);
			out_ptr[bin_length] = INS_MOV;
			++bin_length;
			out_ptr[bin_length] = 01;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			*(unsigned *)(out_ptr + bin_length) = i;
			bin_length += 4;
			
			return 0;
		}
	}
	else if (false) {
	}
	return 0;
}
int Analyse_add(std::string str) {
	unsigned slip_index{ str.find(',') };
	std::string a_str{ str.substr(0,slip_index) };
	std::string b_str{ str.substr(++slip_index,str.length() - slip_index) };
	std::smatch amatch;
	std::smatch bmatch;
	bool f1 = std::regex_match(a_str, amatch, reg0);
	if (f1) {//匹配0 1 2 12  型式命令成功
		bool ftemp = std::regex_match(b_str, bmatch, reg0);
		if (ftemp) {//匹配0型成功 mov R,R  ---->0
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			uint8_t b0 = registernum_to_num(bmatch[1]);
			uint8_t b1 = lowhigh_to_num(bmatch[2]);
			uint8_t b = b0 * 0x10 | b1;
			/*写入out_ptr*/
			out_ptr[bin_length] = INS_ADD;
			++bin_length;
			out_ptr[bin_length] = 0;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			out_ptr[bin_length] = b;
			++bin_length;
			//test_out_ptr(bin_length);
			return 0;
		}
		else if (std::regex_match(b_str, bmatch, reg1))//匹配1型成功 mov R,i   ----->1
		{
			uint8_t a0 = registernum_to_num(amatch[1]);
			uint8_t a1 = lowhigh_to_num(amatch[2]);
			uint8_t a = a0 * 0x10 | a1;
			unsigned i = immed_to_unsgned(bmatch[1], bmatch[2]);
			out_ptr[bin_length] = INS_ADD;
			++bin_length;
			out_ptr[bin_length] = 01;
			++bin_length;
			out_ptr[bin_length] = a;
			++bin_length;
			*(unsigned *)(out_ptr + bin_length) = i;
			bin_length += 4;
			//test_out_ptr(bin_length);
			return 0;
		}
	}
	else if (false) {
	}
	return 0;
	return 0;
}