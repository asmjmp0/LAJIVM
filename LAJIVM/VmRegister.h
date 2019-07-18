#ifndef VMREGISTER_H
#define VMREGISTER_H
#pragma pack(1)
struct vm_register
{
	/*
	*R0-R5Í¨ÓÃ¼Ä´æÆ÷
	*/
	unsigned R0;                    //0
	unsigned R1;                    //1
	unsigned R2;					//2
	unsigned R3;					//3
	unsigned R4;					//4
	unsigned R5;					//5
	//R6µ÷ÊÔ¼Ä´æÆ÷
	unsigned R6;					//6
	unsigned R7;					//7

	unsigned SP;//Õ»¼Ä´æÆ÷			//8
	unsigned BP;//Õ»Ö¡				//9

	unsigned IP;//´úÂëÖ´ĞĞÖ¸Õë		//0xA
	char flag[16];//±êÖ¾¼Ä´æÆ÷		//0xB
};
#pragma pack()
#define REG_NUM 12
#endif // !VMREGISTER_H

