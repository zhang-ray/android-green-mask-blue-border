#pragma once

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>

#include <ui/DisplayInfo.h>
#include <gui/ISurfaceComposer.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>

#include <utils/Log.h>

using namespace android;

class AndroidGreenMaskBlueBorder {
public:
    AndroidGreenMaskBlueBorder() : surfaceComposerClient_(new SurfaceComposerClient()) {}
    ~AndroidGreenMaskBlueBorder() { deInit(); }
    status_t init() {
        sp<IBinder> buildInDisplay(SurfaceComposerClient::getBuiltInDisplay(ISurfaceComposer::eDisplayIdMain));
        DisplayInfo displayInfo;
        auto status = SurfaceComposerClient::getDisplayInfo(buildInDisplay, &displayInfo);
        if (status) {
            ALOGI("SurfaceComposerClient::getDisplayInfo(dtoken, &dinfo);");
            return -1;
        }

        surfaceControl_ = surfaceComposerClient_->createSurface(String8(__FILE__), displayInfo.w, displayInfo.h, PIXEL_FORMAT_RGBA_8888, 0);

        SurfaceComposerClient::openGlobalTransaction();
        surfaceControl_->setLayer(0x40000000);
        surfaceControl_->setAlpha(.5);
        //control->setPosition(100, 100);
        SurfaceComposerClient::closeGlobalTransaction();

        auto surface = surfaceControl_->getSurface();

        const EGLint eglAttribs[] = {
                EGL_RED_SIZE,   8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE,  8,
                EGL_ALPHA_SIZE, 8,
                EGL_DEPTH_SIZE, 0,
                EGL_NONE
        };
        EGLint width, height;
        EGLint eglNumConfigs;
        EGLConfig eglConfig;
        EGLSurface eglSurface;
        EGLContext eglContext;

        EGLDisplay eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

        eglInitialize(eglDisplay, 0, 0);
        eglChooseConfig(eglDisplay, eglAttribs, &eglConfig, 1, &eglNumConfigs);

        EGLint sizeRed, sizeGreen, sizeBlue, sizeAlpha;
        eglGetConfigAttrib(eglDisplay, eglConfig, EGL_RED_SIZE, &sizeRed);
        eglGetConfigAttrib(eglDisplay, eglConfig, EGL_GREEN_SIZE, &sizeGreen);
        eglGetConfigAttrib(eglDisplay, eglConfig, EGL_BLUE_SIZE, &sizeBlue);
        eglGetConfigAttrib(eglDisplay, eglConfig, EGL_ALPHA_SIZE, &sizeAlpha);
        ALOGI("EGL informations:");
        ALOGI("vendor    : %s", eglQueryString(eglDisplay, EGL_VENDOR));
        ALOGI("version   : %s", eglQueryString(eglDisplay, EGL_VERSION));
        ALOGI("extensions: %s", eglQueryString(eglDisplay, EGL_EXTENSIONS));
        ALOGI("Client API: %s", eglQueryString(eglDisplay, EGL_CLIENT_APIS) ? : "Not Supported");
        ALOGI("EGLSurface: %d-%d-%d-%d, config=%p", sizeRed, sizeGreen, sizeBlue, sizeAlpha, eglConfig);
        eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, surface.get(), nullptr);
        eglContext = eglCreateContext(eglDisplay, eglConfig, nullptr, nullptr);
        eglQuerySurface(eglDisplay, eglSurface, EGL_WIDTH, &width);
        eglQuerySurface(eglDisplay, eglSurface, EGL_HEIGHT, &height);

        if (eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext) == EGL_FALSE) {
            return NO_INIT;
        }

        display_ = eglDisplay;
        context_ = eglContext;
        eglDisplaySurface_ = eglSurface;

        surface_ = surface;

        return NO_ERROR;
    }

    void drawOnce() {
        glClearColor(0, .3, 0, .3);
        glClear(GL_COLOR_BUFFER_BIT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        auto theScale = .9999f;
        auto theWidth = 10;

        GLfloat _4lines[] = {
                -theScale,-theScale,+theScale,-theScale,
                -theScale,+theScale,+theScale,+theScale,
                -theScale,-theScale,-theScale,+theScale,
                +theScale,-theScale,+theScale,+theScale
        };

        GLfloat _8colors[] = {
                                0.0f, 0.0f, 1.0f, 1.0f,
                                0.0f, 0.0f, 1.0f, 1.0f,
                                0.0f, 0.0f, 1.0f, 1.0f,
                                0.0f, 0.0f, 1.0f, 1.0f,
                                0.0f, 0.0f, 1.0f, 1.0f,
                                0.0f, 0.0f, 1.0f, 1.0f,
                                0.0f, 0.0f, 1.0f, 1.0f,
                                0.0f, 0.0f, 1.0f, 1.0f
        };
        glLineWidth(theWidth);
        glShadeModel(GL_SMOOTH);
        glVertexPointer(2, GL_FLOAT, 0, _4lines);
        glColorPointer(4, GL_FLOAT, 0, _8colors);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glDrawArrays(GL_LINES, 0, 8);
        glFlush();

        EGLBoolean res = eglSwapBuffers(display_, eglDisplaySurface_);
        if (res) {
            // TODO?
        }
    }


    void deInit() {
        eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(display_, context_);
        eglDestroySurface(display_, eglDisplaySurface_);

        if (surface_ != nullptr) {
            surface_.clear();
        }
        if (surfaceControl_ != nullptr) {
            surfaceControl_.clear();
        }

        eglTerminate(display_);
        eglReleaseThread();
    }

private:
    sp<SurfaceComposerClient> surfaceComposerClient_ = nullptr;
    sp<SurfaceControl> surfaceControl_ = nullptr;
    sp<Surface> surface_ = nullptr;
    EGLDisplay  display_;
    EGLDisplay  context_;
    EGLDisplay  eglDisplaySurface_;
};

