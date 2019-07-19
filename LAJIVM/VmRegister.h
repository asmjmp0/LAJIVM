#ifndef VMREGISTER_H
#define VMREGISTER_H
#pragma pack(1)
struct vm_register
{
	/*
	*R0-R5通用寄存器
	*/
	unsigned R0;                    //0
	unsigned R1;                    //1
	unsigned R2;					//2
	unsigned R3;					//3
	unsigned R4;					//4
	unsigned R5;					//5
	//R6调试寄存器
	unsigned R6;					//6
	unsigned R7;					//7

	unsigned SP;//栈寄存器			//8
	unsigned BP;//栈帧				//9

	unsigned IP;//代码执行指针		//0xA
	char flag[16];//标志寄存器		//0xB
	/*
	*操作类型 0位为0标志位 
	*         1位为有符号大小标志位
	*         15位为单步标志位 
	*		  14位为段标志位
	*/
};
#pragma pack()
#define REG_NUM 12
#endif // !VMREGISTER_H

