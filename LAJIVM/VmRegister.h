#ifndef VMREGISTER_H
#define VMREGISTER_H
#pragma pack(1)
struct vm_register
{
	/*
	*R0-R5ͨ�üĴ���
	*/
	unsigned R0;                    //0
	unsigned R1;                    //1
	unsigned R2;					//2
	unsigned R3;					//3
	unsigned R4;					//4
	unsigned R5;					//5
	//R6���ԼĴ���
	unsigned R6;					//6
	unsigned R7;					//7

	unsigned SP;//ջ�Ĵ���			//8
	unsigned BP;//ջ֡				//9

	unsigned IP;//����ִ��ָ��		//0xA
	char flag[16];//��־�Ĵ���		//0xB

	unsigned CS;                 //0xC
	unsigned DS;               //0xd
	/*
	*�������� 0λΪ0��־λ 
	*         1λΪ�з��Ŵ�С��־λ
	*         15λΪ������־λ 
	*		  14λΪ�α�־λ
	*/
};
#pragma pack()
#define REG_NUM 14
#endif // !VMREGISTER_H

