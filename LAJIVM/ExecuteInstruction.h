#ifndef EXECUTEINSTRUCTION_H
#define EXECUTEINSTRUCTION_H
#define INS_MOV 80
#define INS_LEA 81
#define INS_INT '\xe0'
#define LVM_PRINT '\x10'
#define LVM_EXIT '\x40'
#define INS_JMP 0x40
#define INS_JZ 0x41
#define INS_JNZ 0x42
#define INS_JH 0x43
#define INS_JL 0x44
#define INS_INC 0x30
#define INS_DEC 0x31
#define INS_CMP 0x32
int get_ins_len(char, char);
int _ins_len(char);
int exectue_ins();
void lvm_debug(int ins_len);
int set_flag(char);
int set_flag(short);
int set_flag(unsigned);
int do_ins();
int do_mov();
int do_lea();
int do_int();
int do_jmp();
int do_jz();
int do_jnz();
int do_jh();
int do_jl();
int do_inc();
int do_dec();
int do_cmp();
#endif // !EXECUTEINSTRUCTION_H

