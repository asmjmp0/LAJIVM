#ifndef ANALYSECODE_H
#define ANALYSECODE_H
#include<string>
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
int Analyse_code(std::string);
int Analyse_mov(std::string);
int Analyse_add(std::string);
#endif // !ANALYSECODE_H
