LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE           := blackbox
LOCAL_SRC_FILES        := blackbox.cpp
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE           := test
LOCAL_SRC_FILES        := test.cpp
LOCAL_SHARED_LIBRARIES := blackbox
LOCAL_LDFLAGS          := -llog
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES  := main.cpp
LOCAL_MODULE     := linker-test
include $(BUILD_EXECUTABLE)
