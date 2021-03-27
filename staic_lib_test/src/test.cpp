//
// Created by jmp0 on 2021/3/27.
//
#include "../staticExport/staticExport.h"
#include <stdio.h>
int main(int argc,char** argv){
    init_vm();
    int fib_index = 32;
    clock_t begin = clock();
    execute_code((char *)"TEFKSVZN/0RBVEE6MTIzNENPREU6YAYfAAAAUCEyAAAAAFADMgJQIBICUCEAMAAAAOAhXzIBAgAAAABCBjIAAABhBgAAAAAQBRIQBSIQBTIyAQIBAAAAQQaTAAAAMgECAgAAAEEG"
                         "kwAAAFAgMgIhAQIBAAAAYAYfAAAAUCASAlAgAjIhAQICAAAAYAYfAAAAUCAiAlAgAhIgAAIiETIRIhESYQYAAAAAUCECAQAAABEyESIREmEGAAAAAF8BAQEBAQE=%",1,fib_index);
    clock_t end = clock();
    printf("fib_index:%d\nfib_value:%d\n",fib_index,ret_value);
    printf("time taken:%fms\n",(float )((float)(end-begin))/1000);
}