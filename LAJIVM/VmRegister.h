#ifndef VMREGISTER_H
#define VMREGISTER_H
#pragma pack(1)
struct vm_register
{
	/*
	*R0-R5Í¨ÓÃ¼Ä´æÆ÷
	*/
	unsigned R0;
	unsigned R1;
	unsigned R2;
	unsigned R3;
	unsigned R4;
	unsigned R5;
	//R6µ÷ÊÔ¼Ä´æÆ÷
	unsigned R6;

	unsigned SP;//Õ»¼Ä´æÆ÷
	unsigned BP;//Õ»Ö¡

	unsigned IP;//´úÂëÖ´ĞĞÖ¸Õë
	char flag[8];//±êÖ¾¼Ä´æÆ÷
};
#pragma pack()
#define REG_NUM 11
#endif // !VMREGISTER_H

