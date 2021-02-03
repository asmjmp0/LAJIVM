#include"JITexec.h"
#include"bin_read.h"
#include<fstream>
#include<ios>
#ifdef _WIN32
    #include<Windows.h>
    #include<iostream>
#endif // _WIN32
#ifdef __APPLE__
    #include <TargetConditionals.h>
    #if (defined(TARGET_OS_OSX)) and (defined(TARGET_CPU_ARM64))
        #include <sys/mman.h>
    #endif
#endif


typedef void(*shell)(void);
unsigned len;
unsigned code_index;
unsigned code_bin_end;
char jit_type;
char* shellcode_begin;
unsigned r0, r1, r2, r3, r4, r5, ip;
bool get_jit_file() {
	char *shcode_origin;
	std::ifstream in;
	if(Directory=="") in.open( FileName + ".jit", std::ifstream::in);
	else in.open(Directory+"\\"+FileName+".jit", std::ifstream::in);
	if (in.is_open()) {
		in.seekg(0, std::ios::end);//移到文件尾部
		int shcode_size = in.tellg();//获取文件大小
		shcode_origin = new char[shcode_size];
		in.seekg(0, std::ios::beg);//移到文件头部
		in.read(shcode_origin, shcode_size);
		in.close();
		len = *(unsigned*)(shcode_origin + 3);
		code_index = *(unsigned*)(shcode_origin + 7);
		jit_type = *(shcode_origin + 11);
		code_bin_end= *(unsigned*)(shcode_origin + 12);
		shellcode_begin = shcode_origin + 16;
		if(code_index==registe_ptr->IP)return true;
		else return false;
	}
	else return false;
}
void push_register() {
	r0 = registe_ptr->R0;
	r1 = registe_ptr->R1;
	r2 = registe_ptr->R2;
	r3 = registe_ptr->R3;
	r4 = registe_ptr->R4;
	r5 = (unsigned long )data_ptr;
	ip = registe_ptr->IP;
}
void pop_register() {
	registe_ptr->R0 = r0;
	registe_ptr->R1 = r1;
	registe_ptr->R2 = r2;
	registe_ptr->R3 = r3;
	registe_ptr->R4 = r4;
	if (jit_type == 1) {
		unsigned re_adr = *(unsigned*)(stack_ptr + registe_ptr->SP);
		registe_ptr->SP -= 4;
		registe_ptr->IP = re_adr;
	}
	else if (jit_type == 0) {
		registe_ptr->IP = code_bin_end + registe_ptr->CS;
	}
}
void do_jit() {
#ifdef _WIN32
	int protect_flag;
	VirtualProtectEx(GetCurrentProcess(), shellcode_begin, len, PAGE_EXECUTE_READWRITE, (PDWORD)&protect_flag);
	shell sh;
	sh = (shell)(int)(shellcode_begin);
	push_register();
	_asm {
		mov eax, r0
		mov ebx, r1
		mov ecx, r2
		mov edx, r3
		mov esi,r4
		mov edi,r5
	}
	sh();
	_asm {
		mov r0, eax
		mov r1, ebx
		mov r2, ecx;
		mov r3, edx
		mov r4 ,esi 
		mov r5,edi 
	}
	pop_register();

#endif // _WIN32
}