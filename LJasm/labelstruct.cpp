#include"AnalyseCode.h"
#include"labelstruct.h"
#include"LJasm.h"
int is_label_define(std::string str, int size) {
	if (size == 0)return -1;
	for (int i = 0; i < size; i++)
	{
		if (str == label_s[i].name) {
			return label_s[i].now_bin_length;
		}
	}
	return -1;
}
int write_all_to_jmp() {
	bool findflag = false;
	for (int  m = 0; m <write_label_index; m++)
	{
		for (int  n = 0; n < label_index; n++)
		{
			if (label_s[n].name == write_label_d[m].name) {
				findflag = true;
				int write_index = write_label_d[m].bin_to_write;
				out_ptr[write_index] = write_label_d[m].op;
				out_ptr[write_index + 1] = 0x06;
				*(unsigned*)(out_ptr + write_index + 2) = (unsigned)label_s[n].now_bin_length;
				break;
			}
		}
		if (!findflag) {
			std::cout << "Î´Ê¶±ð±êÇ©:  " <<write_label_d[m].name<<std::endl;
			exit(-1);
		}
		else findflag = false;
	}
}