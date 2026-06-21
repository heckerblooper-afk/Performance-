LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := zygisk_perf_suite
LOCAL_SRC_FILES := main.cpp config_parser.cpp cpu_tuning.cpp gpu_tuning.cpp spoof_engine.cpp fps_unlock.cpp
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_CFLAGS := -Wall -Wextra -Werror -fvisibility=hidden -std=c++17
LOCAL_LDFLAGS := -llog -ldl

include $(BUILD_SHARED_LIBRARY)
