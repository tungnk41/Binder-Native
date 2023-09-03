LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := binder
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := src/main.cpp src/Server.cpp src/Client.cpp src/Interface.cpp
LOCAL_SHARED_LIBRARIES := libutils libcutils libbinder liblog
LOCAL_C_INCLUDES += frameworks/native/include system/core/include $(LOCAL_PATH)/include 
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include
include $(BUILD_EXECUTABLE)