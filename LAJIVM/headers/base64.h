//
// Created by jmp0 on 2021/3/26.
//

#ifndef LAJIVM_BASE64_H
#define LAJIVM_BASE64_H

#include <cstdlib>
#include <cstring>

unsigned char *base64_encode(unsigned char *str,int str_len);

unsigned char *base64_decode(unsigned char *code);
#endif //LAJIVM_BASE64_H
