#ifndef _MTEGL_H_
#define _MTEGL_H_
#include "mtdefine.h"
#ifdef PLATFORM_ANDROID
#include <EGL/egl.h>	//for egl
#include <EGL/eglplatform.h>
#include <GLES3/gl3.h>
#include <GLES3/gl31.h>
#include <GLES3/gl32.h>
#elif defined(PLATFORM_WINDOWS)
#include <Windows.h>
#include <gl/glew.h>
#endif
typedef struct EGLEnvionment {
#ifdef PLATFORM_ANDROID
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
#elif defined(PLATFORM_WINDOWS)
	HGLRC context;
	HWND windows;
#endif
    int width;
    int height;
}EGLEnvionment;
class EGLUtil{
public:
#ifdef PLATFORM_ANDROID
	static int egl_init_display(struct EGLEnvionment* eglEnvionment,int envWidth,int envHeight, EGLContext eglContext = EGL_NO_CONTEXT);
#elif defined(PLATFORM_WINDOWS)
	static int egl_init_display(struct EGLEnvionment* eglEnvionment, int envWidth, int envHeight, HGLRC context = NULL);

#endif
	static void egl_term_display(struct EGLEnvionment* eglEnvionment);
private:
};
#endif

