#ifndef BIN_READ_H
#define BIN_READ_H
#include"init.h"
#include<string>
extern char origin_bin[0xffff];
int read_bin(std::string str);
int write_to_data();
int write_to_code();
bool bincmp(char *, char *, int);
int find_str(char*, char*,int ,int);
void write_all(std::string str = bin_path);
#define BIN_LENGTH 200
#endif // !BIN_READ_H

