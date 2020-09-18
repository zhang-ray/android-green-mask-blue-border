LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= AndroidGreenMaskBlueBorderStressTest.cpp \
    AGMBB.cpp 

LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES

LOCAL_SHARED_LIBRARIES := \
    libui \
    libgui \
    libEGL \
    liblog \
    libutils \
    libGLESv1_CM

LOCAL_MODULE:= android-green-mask-blue-border-stress-test


include $(BUILD_EXECUTABLE)
