#include <jni.h>

#include <arm_neon.h>

#include <math.h>

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


extern "C"
void unsigned_sub_result_signed(){
    uint8x8_t a = vhsub_u8 (vdup_n_u8(4),vdup_n_u8(6) ); // h = halve
    uint8_t result[8];
    vst1_u8(result, a);
    LOGW("vhsub_u8 =  %x", result[0]); // ff


    uint8_t a1 = 4;
    uint8_t a2 = 6;
    uint8_t a3 = a1 - a2 ;
    LOGE("uint8_t = %x ", a3 ); // fe = -2 的补码
}


extern "C"
void narrow_add(){
    // int8x8_t vraddhn_s16 (int16x8_t __a, int16x8_t __b);


    {
        // Vector add high half(窄指令): vaddhn -> ri = sat(ai + bi);
        // selecting High half, The results are truncated
        // h 是 high half 不是half
        int8x8_t a = vaddhn_s16 (vdupq_n_s16(255),vdupq_n_s16(1) );
        uint8_t result[8];
        vst1_u8(result, a);
        LOGW("[s16->s8 high haft][255+1] %x", result[0]); // 0x1
    }

    {
        int16_t a = 32767;
        int16_t b = 1 ;
        int16_t c = a + b ;
        LOGW("[32767+1] %x %d", c, c); // ffff8000 -32768
    }
    {
        int8x8_t a = vaddhn_s16 (vdupq_n_s16(32767),vdupq_n_s16(1) );
        uint8_t result[8];
        vst1_u8(result, a);
        LOGW("[s16->s8 high haft][32767+1] %x", result[0]); // 0x80
    }

    {
        /*
         *
         *  "vqdmulhq" = "v" + "q" + "d" + "mul" + "h" + "q"
         *  b和c是16位的话 饱和(b * c * 2) >> 16
         *
         *  b*c*2 计算的结果按照32bits 然后饱和 再取32bits的高16bits
         *
            "v" = "vector"，向量。
            "q" = 饱和。
            "d" = "doubling"，双倍。
            "mul" = 乘法。
            "h" = "high"，高位。
            "q" = QWORD，全字，128位。

         */
        int16x4_t a = vqdmulh_s16(vdup_n_s16(16384),vdup_n_s16( (int16_t)pow(2,5)) );
        int16_t result[4];
        vst1_s16(result, a);
        LOGW("[饱和(b * c * 2作为S32来做饱和) >> 16] %x %x %x %x ",
             result[0],result[1],result[2],result[3]); // 0x10
    }


    {
        /*

            "vqrdmulhq" = "v" + "r" + "q" + "d" + "mul" + "h" + "q"

            b和c是16位的话  a = 饱和(b * c * 2 + 2^15) >> 16

            b*c*2 计算的结果按照32bits 然后饱和 再取32bits的高16bits

            "v" = "vector"，向量。
            "r" = "rounding"，舍入。
            "q" = 饱和。
            "d" = "doubling"，双倍。
            "mul" = 乘法。
            "h" = "high"，高位。
            "q" = QWORD，全字，128位

         */
        // 28672 = 0111 0000 0000 0000
        int16x4_t a = vqrdmulh_s16(vdup_n_s16(28672),vdup_n_s16( (int16_t)pow(2,2)) );
        int16_t result[4];
        vst1_s16(result, a);
        LOGW("[a = 饱和(b * c * 2 + 2^15) >> 16] %x %x %x %x ",
             result[0],result[1],result[2],result[3]); // 0x4
    }


}

