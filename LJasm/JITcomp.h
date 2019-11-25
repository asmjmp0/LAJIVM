#ifndef JITCOMP_H
#define JITCOMP_H
#include<iostream>
#include<string>
extern int jit_begin;
extern int jit_end;
int Analyse_jit_in();
int Analyse_jit_out();
void jit_str_init();
void jit_macro_change(std::string &str);
void jit_create_jit_file();
#endif // !JITCOMP_H

