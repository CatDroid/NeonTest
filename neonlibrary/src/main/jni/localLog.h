//
// Created by Hanlon on 2019/4/6.
//

#ifndef NEONTEST_LOCALLOG_H
#define NEONTEST_LOCALLOG_H

#include <android/log.h>

#include <stdio.h>

#define TAG "neon"

#define LOGE(format,args...) __android_log_print(ANDROID_LOG_ERROR,TAG,format,##args)
#define LOGW(format,args...) __android_log_print(ANDROID_LOG_WARN,TAG,format,##args)


//#define LOGE(format,args...) printf("[E][%s][%d]" format "\n", __FUNCTION__,__LINE__,##args)
//#define LOGW(format,args...) printf("[W][%s][%d]" format "\n", __FUNCTION__,__LINE__,##args)

#endif //NEONTEST_LOCALLOG_H
