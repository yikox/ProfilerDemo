#include "mtegl.h"
#include <errno.h>
#include <malloc.h>
#include <stdio.h>

#ifndef LOGE
#include <android//log.h>
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "RLog", __VA_ARGS__))
#endif

#ifdef PLATFORM_WINDOWS
LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, Message, wParam, lParam);
}
#endif

/**
 * Initialize an EGL context for the current display.
 */
#ifdef PLATFORM_ANDROID
int EGLUtil::egl_init_display(struct EGLEnvionment* eglEnvionment, int envWidth, int envHeight, EGLContext share_context/* = EGL_NO_CONTEXT*/)
#elif defined(PLATFORM_WINDOWS)
int EGLUtil::egl_init_display(struct EGLEnvionment* eglEnvionment, int envWidth, int envHeight,HGLRC share_context/* = NULL*/)
#endif
{
    // initialize OpenGL ES and EGL
	
#ifdef PLATFORM_ANDROID
    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */


   const EGLint attribs[] = {
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,//EGL_WINDOW_BIT,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8, 
			EGL_ALPHA_SIZE,8,
            EGL_NONE
    };/**/
    EGLint w, h, dummy, format;
    EGLint numConfigs;
    EGLConfig config;
    EGLSurface surface;
    EGLContext context;

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    EGLint majorVersion,minorVersion;
    eglInitialize(display, &majorVersion, &minorVersion);
	//LOGD("zcd eglInitialize majorVersion = %d, minorVersion = %d",majorVersion,minorVersion);
	
	
	EGLBoolean success = EGL_FALSE;
    /* Here, the application chooses the configuration it desires. In this
     * sample, we have a very simplified selection process, where we pick
     * the first EGLConfig that matches our criteria */
	success = eglGetConfigs(display,NULL,0,&numConfigs);
	if(success == EGL_TRUE && numConfigs > 0)
    {
		//LOGD("zcd egl_YES_surface =");

	    EGLBoolean i = eglChooseConfig(display, attribs, &config, 1, &numConfigs);

		//LOGD("zcd egl_no_surface = %0x,%d",eglGetError(),i);
	}
	else
	{
		LOGE("eglGetConfigs failed");

		return -1;
	}
    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
   // eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

    //ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

    //surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);
	const GLint atrrilist[] = {
		EGL_WIDTH,envWidth,
		EGL_HEIGHT,envHeight,
		EGL_LARGEST_PBUFFER,EGL_TRUE,
		EGL_NONE
	};

	//LOGD("zcd eglCreatePbufferSurface");
    surface = eglCreatePbufferSurface(display,config,(const EGLint*)atrrilist);
	//LOGD("zcd eglCreatePbufferSurface1");
	if(surface == NULL)
	{
		//EGL_BAD_CONFIG
		EGLint i = eglGetError();
		LOGE("zcd egl_no_surface = %0x",i);
		return -1;
	}

	const EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
	//LOGD("zcd egl_no_contextAttribs = %0x",eglGetError());
    context = eglCreateContext(display, config,  share_context, contextAttribs);
	if(context == NULL)
	{
		//EGL_BAD_CONFIG
		EGLint i = eglGetError();
		LOGE("zcd egl_no_context = %0x",i);
		return -1;
	}
    
	//LOGD("zcd eglMakeCurrent");
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        LOGE(" zcd Unable to eglMakeCurrent");
        return -1;
    }

	//LOGD("zcd shader1");
    eglQuerySurface(display, surface, EGL_WIDTH, &w);
    eglQuerySurface(display, surface, EGL_HEIGHT, &h);
	//LOGD("zcd shader2 = %d,%d,err = %0x",w,h,eglGetError());

    eglEnvionment->display = display;
    eglEnvionment->context = context;
    eglEnvionment->surface = surface;
    eglEnvionment->width = w;
    eglEnvionment->height = h;
#elif defined(PLATFORM_WINDOWS)
	WNDCLASSEX wndClass;
	wndClass.cbSize         = sizeof(WNDCLASSEX);
	wndClass.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	wndClass.lpfnWndProc        = WndProc;
	wndClass.cbClsExtra     = 0;
	wndClass.cbWndExtra     = 0;
	wndClass.hInstance      = 0;
	wndClass.hIcon          = 0;
	wndClass.hCursor        = LoadCursor(0, IDC_ARROW);
	wndClass.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName   = 0;
	wndClass.lpszClassName  = L"WndClass";
	wndClass.hIconSm        = 0;
	RegisterClassEx(&wndClass);
	// Style the window and remove the caption bar (WS_POPUP)
	DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;

	// Create the window. Position and size it.
	eglEnvionment->windows = CreateWindowEx(0,
		L"WndClass",
		L"",
		style,
		CW_USEDEFAULT, CW_USEDEFAULT, envWidth, envHeight,
		0, 0, 0, 0);
	HDC dc = GetDC(eglEnvionment->windows);
	// Setup OpenGL
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 16;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pixelFormat, &pfd);
	
	eglEnvionment->context = wglCreateContext(dc);
	if (share_context)
	{
		LOGD("unuse shared context in windows.");
	}
	wglMakeCurrent(dc, eglEnvionment->context);
#endif
    return 0;
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void EGLUtil::egl_term_display(struct EGLEnvionment* eglEnvionment) {
#ifdef PLATFORM_ANDROID
    if (eglEnvionment->display != EGL_NO_DISPLAY) {
        eglMakeCurrent(eglEnvionment->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (eglEnvionment->context != EGL_NO_CONTEXT) {
            eglDestroyContext(eglEnvionment->display, eglEnvionment->context);
        }
        if (eglEnvionment->surface != EGL_NO_SURFACE) {
            eglDestroySurface(eglEnvionment->display, eglEnvionment->surface);
        }
        eglTerminate(eglEnvionment->display);
    }

    eglEnvionment->display = EGL_NO_DISPLAY;
    eglEnvionment->context = EGL_NO_CONTEXT;
    eglEnvionment->surface = EGL_NO_SURFACE;
#elif defined(PLATFORM_WINDOWS)
	if(wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL,NULL);
	if(eglEnvionment->context != NULL)
	{
		wglDeleteContext(eglEnvionment->context);
		eglEnvionment->context = NULL;
	}
	if(eglEnvionment->windows != NULL)
	{
		DestroyWindow(eglEnvionment->windows);
		eglEnvionment->windows = NULL;
	}
#endif
}
