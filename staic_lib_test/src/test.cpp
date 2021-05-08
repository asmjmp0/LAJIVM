//
// Created by jmp0 on 2021/3/27.
//
#include "../staticExport/staticExport.h"
#include <unistd.h>

//此代码1～num求和

#ifdef __clang__
#pragma clang optimize off
#elif defined(__GNUC__)
#pragma GCC push_options
#pragma GCC optimize ("O0")
#elif defined(_MSC_VER)
#pragma optimize("", off)
#endif
int sum(int num){
    int _sum = 0;
    for (int i = 0; i <= num; ++i)
        _sum = _sum + i;
    return _sum;
}
int fib(int x){
    if(x == 0)return 0;
    if(x == 1)return 1;
    if(x == 2) return 1;
    return fib(x-1) + fib(x-2);
}
#ifdef __clang__
#pragma clang optimize on
#elif defined(__GNUC__)
#pragma GCC pop_options
#elif defined(_MSC_VER)
#pragma optimize("", on)
#endif
//此代码为斐波拉契数列递归计算
int main(int argc,char** argv){
    init_vm();
    int count = 900000;
    int num = 32;
    //命令行测试
    if(argc==3){
        count = atoi(argv[1]);
        num = atoi(argv[2]);
    }
#if defined(__APPLE__) || defined(__linux__)
    system("uname -srm");
#elif defined(_WIN32)
    system("ver");
#endif
    clock_t begin = clock();
    //此代码1～num求和
    execute_code((char *)"TEFKSVZN/0RBVEE6Q09ERTpQIBICMRIgAAISMgESAAAAAEIGBAAAAFAgEgJQIQAwAAAA4CEBAQEBAQE=",1,count);
    clock_t sum_begin = clock();
    int temp_sum = sum(count);
    clock_t sum_end = clock();
    clock_t middle = clock();
    float sum_origin_time = ((float)(sum_end-sum_begin))/1000;
    printf("sum:%d\n",ret_value);
    printf("%fms,%fms\n",sum_origin_time,((float)(middle-begin))/1000 - sum_origin_time);

    //此代码为斐波拉契数列递归计算
    execute_code((char *)"TEFKSVZN/0RBVEE6MTIzNENPREU6YAYfAAAAUCEyAAAAAFADMgJQIBICUCEAMAAAAOAhXzIBAgAAAABCBjIAAABhBgAAAAAQBRIQBSIQBTIyAQIBAAAAQQaTAAAAMgECAgAAAEEG"
                         "kwAAAFAgMgIhAQIBAAAAYAYfAAAAUCASAlAgAjIhAQICAAAAYAYfAAAAUCAiAlAgAhIgAAIiETIRIhESYQYAAAAAUCECAQAAABEyESIREmEGAAAAAF8BAQEBAQE=",1,num);
    clock_t fib_begin = clock();
    int temp_fib= fib(num);
    clock_t fib_end = clock();
    clock_t end = clock();
    float fib_origin_time = ((float)(fib_end-fib_begin))/1000;
    printf("fib:%d\n",ret_value);
    printf("%fms,%fms\n",fib_origin_time,((float)(end-middle))/1000 - fib_origin_time);
}
