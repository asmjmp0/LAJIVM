#ifndef BIN_READ_H
#define BIN_READ_H
#include"init.h"
#include <string>
#include <cstring>
extern char origin_bin[0xffff];
extern unsigned char file_chara;
extern std::string FileName;
extern std::string Directory;
extern unsigned d_adr;
extern unsigned c_adr;
int read_bin(std::string str);
int write_to_data();
int write_to_code();
bool bincmp(char *, const char *, int);
int find_str(char *, const char *,int ,int);
void write_all(std::string str = bin_path);
#endif // !BIN_READ_H

