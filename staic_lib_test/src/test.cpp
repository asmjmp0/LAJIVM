//
// Created by jmp0 on 2021/3/27.
//
#include "../staticExport/staticExport.h"
int main(int argc,char** argv){
    init_vm();
    int num = 20;
    clock_t begin = clock();
    //此代码1～num求和
    execute_code((char *)"TEFKSVZN/0RBVEE6Q09ERTpQIBICMRIgAAISMgESAAAAAEIGBAAAAFAgEgJQIQAwAAAA4CEBAQEBAQE=",1,num);
    printf("num:%d ret_value:%d\n",num,ret_value);

    //此代码为斐波拉契数列递归计算
    execute_code((char *)"TEFKSVZN/0RBVEE6MTIzNENPREU6YAYfAAAAUCEyAAAAAFADMgJQIBICUCEAMAAAAOAhXzIBAgAAAABCBjIAAABhBgAAAAAQBRIQBSIQBTIyAQIBAAAAQQaTAAAAMgECAgAAAEEG"
                         "kwAAAFAgMgIhAQIBAAAAYAYfAAAAUCASAlAgAjIhAQICAAAAYAYfAAAAUCAiAlAgAhIgAAIiETIRIhESYQYAAAAAUCECAQAAABEyESIREmEGAAAAAF8BAQEBAQE=",1,num);
    
    printf("num:%d ret_value:%d\n",num,ret_value);
    clock_t end = clock();
    printf("time taken:%fms\n",((float)(end-begin))/1000);
}
