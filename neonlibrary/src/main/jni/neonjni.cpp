//
// Created by Hanlon on 2019/4/7.
//


#include <jni.h>
#include <inttypes.h>

#include <cpu-features.h>

#include "localLog.h"

#include "neonnative.h"


extern "C"
JNIEXPORT void JNICALL
Java_com_example_neonlibrary_NeonJNI_runNeon(JNIEnv *env,
                                                    jclass type,
                                                    jobject in,
                                                    jint inSize,
                                                    jobject out,
                                                    jint outSize) {

    uint8_t* input = (uint8_t*)env->GetDirectBufferAddress(in);
    uint8_t* output = (uint8_t*)env->GetDirectBufferAddress(out);


    convertRGBA2BGRA(input,output,inSize);

}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_neonlibrary_NeonJNI_runNormal(JNIEnv *env,
                                                      jclass type,
                                                      jobject in,
                                                      jint inSize,
                                                      jobject out,
                                                      jint outSize ) {

    // TODO

}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_neonlibrary_NeonJNI_neonMul(JNIEnv *env, jclass type, jfloatArray a_,
                                             jfloatArray b_, jfloatArray result_) {
    jfloat *a = env->GetFloatArrayElements(a_, NULL);
    jfloat *b = env->GetFloatArrayElements(b_, NULL);
    jfloat *result = env->GetFloatArrayElements(result_, NULL);
    // JNI 得到的 GetFloatArrayElements 也是 8字节对齐的 ?? 目前测试是的

    mul2_neon(a,b,result);

    env->ReleaseFloatArrayElements(a_, a, 0);
    env->ReleaseFloatArrayElements(b_, b, 0);
    env->ReleaseFloatArrayElements(result_, result, 0);
    return JNI_TRUE ;
}



extern "C"
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {


    /*
        获取CPU个数
        extern int android_getCpuCount(void);

        设置CPU个数?沙盒模拟测试
        extern int android_setCpu(int cpu_count, uint64_t cpu_features);
        extern uint32_t android_getCpuIdArm(void);
        extern int android_setCpuArm(int cpu_count, uint64_t cpu_features, uint32_t cpu_id);
    */

    LOGE("cpu core is %d ", android_getCpuCount());
    LOGE("cpu family %d ", android_getCpuFamily());
    LOGE("cpu features is %" PRId64, android_getCpuFeatures());

    if (android_getCpuFamily() == ANDROID_CPU_FAMILY_ARM &&
        (android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON) != 0) {
        // 不同的cpu架构,比如arm和armv8 会返回feature的定义都不一样

        // use NEON-optimized routines
        LOGE("Support NEON(ARM Advance SIMD)");
    } else {
        // use non-NEON fallback routines instead
        LOGE("do NOT support NEON(ARM Advance SIMD)");

    }


    return JNI_VERSION_1_4; // 必须返回JNI版本号
}

