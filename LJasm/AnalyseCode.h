#ifndef ANALYSECODE_H
#define ANALYSECODE_H
#include<string>
#include<regex>
#define INS_PUSH 0x10
#define INS_POP 0x11
#define INS_MOV 0x50
#define INS_LEA 0x51
#define INS_INT 0xe0
#define INS_JMP 0x40
#define INS_JZ 0x41
#define INS_JNZ 0x42
#define INS_JH 0x43
#define INS_JL 0x44
#define INS_INC 0x30
#define INS_DEC 0x31
#define INS_CMP 0x32
#define INS_ADD 0x20
#define INS_SUB 0x21
#define INS_XOR 0x22
#define INS_AND 0x23
#define INS_OR 0x24
#define INS_NOT 0x25
#define INS_NOP 0x5f
#define INS_CALL 0x60
#define INS_RET 0x61
uint8_t registernum_to_num(std::string);
uint8_t lowhigh_to_num(std::string);
unsigned immed_to_unsgned(std::string, std::string);
std::string& trim(std::string &s);
void string_replace(std::string &strBig, const std::string &strsrc, const std::string &strdst);
void Macro_str(std::string&);
int Analyse_code(std::string);
int Analyse_mov(std::string);
int Analyse_add(std::string);
int Analyse_lea(std::string);
int Analyse_int(std::string);
int Analyse_cmp(std::string);
int Analyse_jmp(std::string);
int Analyse_jnz(std::string);
int Analyse_jz(std::string);
int Analyse_jl(std::string);
int Analyse_jh(std::string);
int Analyse_inc(std::string);
int Analyse_dec(std::string);
int Analyse_not(std::string);
int Analyse_sub(std::string);
int Analyse_xor(std::string);
int Analyse_and(std::string);
int Analyse_or(std::string);
int Analyse_push(std::string);
int Analyse_pop(std::string);
int Analyse_call(std::string);
int Analyse_ret(std::string);
#endif // !ANALYSECODE_H
