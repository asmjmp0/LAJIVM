//
// Created by jmp0 on 2021/3/27.
//
#include "../staticExport/staticExport.h"
#include <unistd.h>
//此代码1～num求和
int sum(int num){
    int _sum = 0;
    for (int i = 0; i <= num; ++i)
        _sum = _sum + i;
    return _sum;
}
//此代码为斐波拉契数列递归计算
int fib(int x){
    if(x == 0)return 0;
    if(x == 1)return 1;
    if(x == 2) return 1;
    return fib(x-1) + fib(x-2);
}
int main(int argc,char** argv){
    init_vm();
    int num = 32;
#if defined(__APPLE__) || defined(__linux__)
    system("uname -a");
#elif
    system("ver");
#endif
    clock_t begin = clock();
    //此代码1～num求和
    printf("1～20求和\n");
    execute_code((char *)"TEFKSVZN/0RBVEE6Q09ERTpQIBICMRIgAAISMgESAAAAAEIGBAAAAFAgEgJQIQAwAAAA4CEBAQEBAQE=",1,num);
    printf("num:%d ret_value:%d\n",num,ret_value);
    clock_t sum_begin = clock();
    assert(ret_value == sum(num));
    clock_t sum_end = clock();
    clock_t middle = clock();
    printf("time taken:%fms,%fms\n",((float)(sum_end-sum_begin))/1000,((float)(middle-begin))/1000);

    //此代码为斐波拉契数列递归计算
    printf("斐波拉契数列第20项\n");
    execute_code((char *)"TEFKSVZN/0RBVEE6MTIzNENPREU6YAYfAAAAUCEyAAAAAFADMgJQIBICUCEAMAAAAOAhXzIBAgAAAABCBjIAAABhBgAAAAAQBRIQBSIQBTIyAQIBAAAAQQaTAAAAMgECAgAAAEEG"
                         "kwAAAFAgMgIhAQIBAAAAYAYfAAAAUCASAlAgAjIhAQICAAAAYAYfAAAAUCAiAlAgAhIgAAIiETIRIhESYQYAAAAAUCECAQAAABEyESIREmEGAAAAAF8BAQEBAQE=",1,num);
    printf("num:%d ret_value:%d\n",num,ret_value);
    clock_t fib_begin = clock();
    assert(ret_value == fib(num));
    clock_t fib_end = clock();
    clock_t end = clock();
    printf("time taken:%fms,%fms\n",((float)(fib_end-fib_begin))/1000,((float)(end-middle))/1000);
}
