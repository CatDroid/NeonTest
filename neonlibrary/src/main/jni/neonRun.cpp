

#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include "neonnative.h"
#include "localLog.h"


#define FILE_PATH   "/sdcard/red.rgba"
#define OUTPUT_PATH "/sdcard/red.bgra"
#define WIDTH   1920
#define HEIGHT  1080
#define RGBA_SIZE WIDTH*HEIGHT*4



int test1(){
    LOGE("begin");

    uint8_t* allocBuf = (uint8_t*)malloc(RGBA_SIZE*2);
    uint8_t* outBuf = (uint8_t*)malloc(RGBA_SIZE);

    uint8_t* imgBuf = allocBuf + 3 ; // 非16bits对齐

    printf("main enter \n");
    int fd = open(FILE_PATH,O_RDONLY,0755);
    if(fd < 0){
        LOGE("open " FILE_PATH " fail %s " ,strerror(errno));
        return -1;
    }
    int ret = read(fd, imgBuf,  RGBA_SIZE );
    if(ret != RGBA_SIZE ){
        LOGE("read fail %s " , strerror(errno));
        return -1;
    }
    close(fd);


    convertRGBA2BGRA(imgBuf,outBuf,RGBA_SIZE);




    int fd1 = open(OUTPUT_PATH,O_WRONLY|O_CREAT|O_TRUNC,0755);
    if(fd1 < 0){
        LOGE("open " OUTPUT_PATH " fail %s " , strerror(errno));
        return -1 ;
    }

    ret = write(fd1, outBuf,  RGBA_SIZE );
    if(ret != RGBA_SIZE ){
        LOGE("write fail %s " , strerror(errno));
        return -1;
    }

    close(fd1);


    free(outBuf);
    free(allocBuf);


    LOGE("end");
    return 0;
}

#define USING_MALLOC
int test2(){

    LOGE("begin");

    float output[2]   = {0};
    float input1[2] __attribute__((aligned(1)))  = {1.0f,2.0f};

    //  float input2[2] __attribute__((aligned(1)))  = {1.0f,2.0f};
    //  float input2[2] __attribute__((aligned(16))) = {1.0f,2.0f};

#if defined(ALIGN_ODD)
    uint8_t noAlignFloat[4*2];
    uint8_t* pNoAlignFloat = noAlignFloat ;
    float* input2 = (float*)(  ((uint32_t)pNoAlignFloat -1) /2 *2 +1);
    input2[0] = 1.0f;
    input2[1] = 2.0f;
    // 得到input2是奇数地址
    // 使用vld1_f32/vst1_f32加载到neon和取出数据 没有问题
    // float32x2_t b = *((float32x2_t*)v1);  会遇到SIGBUS 异常 !!!
#elif defined(USING_MALLOC)
    // malloc在32位编译系统中会8字节对齐，64为编译系统中会8或者16字节对齐
    float* input2 = (float*)malloc( sizeof(float)*2 );
    input2[0] = 1.0f;
    input2[1] = 2.0f;
#else
    uint8_t noAlign = 2 ;
    noAlign++;
    float input2[2]   = {1.0f,2.0f}; // input2分配的地址还是4字节对齐
#endif
    mul2_neon(input1,input2,output);

    LOGW("output %f %f" , output[0], output[1]);

    LOGE("end");
    return 0;

}




int main(int argc, char* argv[]){

    LOGE("TEST START %s " , __TIME__ );


    int32_t ret = 0 ;

    ret += test1();

    ret += test2();


    return 0;
}
