#ifndef  LABELSTRUCT_H
#define LABELSTRUCT_H
#include<string>
#pragma pack(1)
struct label_struct
{
	std::string name;
	int now_bin_length;
};
struct write_label_data
{
	std::string name;
	uint8_t op;
	int bin_to_write;
};
#pragma pack()
int is_label_define(std::string str, int len);
int write_all_to_jmp();
#endif // ! LABELSTRUCT_H

