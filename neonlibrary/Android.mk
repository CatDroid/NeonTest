
ANDROID_MK_PATH:= $(call my-dir)

#########################################################################
include $(CLEAR_VARS)
LOCAL_PATH := $(ANDROID_MK_PATH)

LOCAL_MODULE:= myNeonLib

#LOCAL_LDLIBS += -llog # 静态库忽略LOCAL_LDLIBS
#LOCAL_SHARED_LIBRARIES += libsdkLog
#LOCAL_STATIC_LIBRARIES += cpufeatures

#要支持neon必须带有这个
LOCAL_ARM_NEON := true

LOCAL_SRC_FILES+= \
    $(LOCAL_PATH)/src/main/jni/neonnative.cpp

include $(BUILD_STATIC_LIBRARY)

#########################################################################
include $(CLEAR_VARS)
LOCAL_PATH := $(ANDROID_MK_PATH)

LOCAL_MODULE:= myNeonJNI

LOCAL_LDLIBS += -llog
#LOCAL_LDLIBS += -lz -lGLESv1_CM -lEGL -lGLESv2
#LOCAL_SHARED_LIBRARIES += libsdkLog
LOCAL_STATIC_LIBRARIES += cpufeatures
LOCAL_STATIC_LIBRARIES += myNeonLib

LOCAL_SRC_FILES+= \
    $(LOCAL_PATH)/src/main/jni/neonjni.cpp

include $(BUILD_SHARED_LIBRARY)

#########################################################################

include $(CLEAR_VARS)
LOCAL_PATH := $(ANDROID_MK_PATH)

LOCAL_MODULE:= myNeonRun

LOCAL_CFLAGS += -fPIE
LOCAL_LDFLAGS += -fPIE -pie

LOCAL_LDLIBS += -llog  # -lz -lGLESv1_CM -lEGL -lGLESv2
#LOCAL_SHARED_LIBRARIES += libsdkLog
LOCAL_STATIC_LIBRARIES += cpufeatures
LOCAL_STATIC_LIBRARIES += myNeonLib

LOCAL_SRC_FILES+= \
    $(LOCAL_PATH)/src/main/jni/neonRun.cpp



include $(BUILD_EXECUTABLE)

#########################################################################
# 使用cpufeatures库
$(call import-module,android/cpufeatures)
