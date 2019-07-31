#ifndef LJASM_H
#define LJASM_H
#include<iostream>
#include<string>
#include<vector>
#define BIN_MAX_SIZE_ 5
extern std::vector<std::string> origin_str;
extern int bin_length;
extern int asm_length;
extern int now_index;
extern uint8_t* out_ptr;
void test_out_ptr(int);
bool read_file(char *);
int read_data();
int read_code();
bool write_end_out(std::string);
int string_to_hex(std::string);
#endif // !LJASM_H
