//
// Created by Hanlon on 2019/4/7.
//

#ifndef NEONTEST_NEONNATIVE_H
#define NEONTEST_NEONNATIVE_H


#include <stdint.h>

extern "C"
bool convertRGBA2BGRA(uint8_t* input,uint8_t* output ,int32_t size );



extern "C"
void mul2_neon(const float v0[2], const float v1[2], float out[2]);


extern "C"
void unsigned_sub_result_signed();


extern "C"
void narrow_add();

#endif //NEONTEST_NEONNATIVE_H
