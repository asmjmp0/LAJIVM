#ifndef EXECUTEINSTRUCTION_H
#define EXECUTEINSTRUCTION_H
#define INS_MOV 80
#define INS_LEA 81
#define INS_INT '\xe0'
#define LVM_PRINT '\x10'
#define LVM_EXIT '\x40'
int get_ins_len(char , char );
int _ins_len(char);
int exectue_ins();
int do_ins();
int do_mov();
int do_lea();
int do_int();
#endif // !EXECUTEINSTRUCTION_H

