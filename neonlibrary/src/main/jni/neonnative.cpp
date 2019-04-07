#include <jni.h>

#include <arm_neon.h>

#include "localLog.h"

extern "C"
bool convertRGBA2BGRA(uint8_t* input,uint8_t* output ,int32_t size // =width*height*4
){

    LOGW("input %p output %p size %d", (void*)input, (void*)output, size);

    // VFPv3_D32 Neon 有 32个D寄存器(双精度8字节) 或者 16个Q寄存器(16字节/128位)
    uint8x16x4_t src_vectors; // 8x16=128bit是Q寄存器 并且使用4个
    uint8x16x4_t dst_vectors;

    uint32_t i = 0;
    for (; i < size; i += 4 * 16) {
        //	vld4.8	{d16, d18, d20, d22}, [r0]! // Q
        //  vld4.8	{d17, d19, d21, d23}, [r0]

        //  一次处理16个像素
        src_vectors = vld4q_u8(input + i);// v neon指令 ld4操作    q正常指令 u8返回值类型
        dst_vectors.val[0] = src_vectors.val[2];
        dst_vectors.val[1] = src_vectors.val[1];
        dst_vectors.val[2] = src_vectors.val[0];
        dst_vectors.val[3] = src_vectors.val[3];
        vst4q_u8(output + i, dst_vectors);
    }

    for (; i < size; i += 4) {
        output[i + 0] = input[i + 2];
        output[i + 1] = input[i + 1];
        output[i + 2] = input[i + 0];
        output[i + 3] = input[i + 3];
    }

    return true ;
}

extern "C"
void mul2_neon(const float v0[2], const float v1[2], float out[2])
{
    LOGW("v0 %p v1 %p  out %p ", v0, v1, out);
    float32x2_t a = vld1_f32(v0); // 64bit的D寄存器   vld1q_f32  这个是加载到128bits的Q寄存器
//    float32x2_t a = *((float32x2_t*)v0);
    //float32x2_t b = vld1_f32(v1);
    LOGW("1111");
    float32x2_t b = *((float32x2_t*)v1);
    LOGW("2222");
    float32x2_t result = vmul_f32(a,b);
//    *((float32x2_t*)out) = result ;
    vst1_f32(out, result );
}
