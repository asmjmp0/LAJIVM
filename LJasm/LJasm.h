#ifndef LJASM_H
#define LJASM_H
#include<iostream>
#include<string>
#include<vector>
#include"labelstruct.h"
#define BIN_MAX_SIZE_ 5
extern std::vector<std::string> origin_str;
extern int bin_length;
extern int asm_length;
extern int now_index;
extern uint8_t* out_ptr;
extern label_struct *label_s;
extern write_label_data *write_label_d;
extern int label_index;
extern int write_label_index;
extern int data_long;
extern std::string file_name;
void test_out_ptr(int);
bool read_file(char *);
int read_data();
int read_code();
bool write_end_out(std::string);
int string_to_hex(std::string);
#endif // !LJASM_H
/*
JIT格式
JIT标识符 3byte
shellcode len 4byte ----3
shellcode index 4byte -----7
shellcode type 1byte ------11
shellcode data len byte
*/
