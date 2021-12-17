/*
 * mtdefine.h
 *
 * brief: 各平台的公共宏定义
 */
#ifndef __MTDEFINE_H__
#define __MTDEFINE_H__

// plaform detection
#if defined(_WIN32) || defined(_WIN32_) || defined(WIN32) || defined(_WIN64_) || defined(WIN64) || defined(_WIN64)
    #define PLATFORM_WINDOWS 1
    #if _MSC_VER < 1600 // before Visual Studio 2010 have not <stdint.h>
    #define STDINT_MISSING   1
    #endif
#elif defined(ANDROID) || defined(_ANDROID_)
    #define PLATFORM_ANDROID 1
    #if !defined(__clang__) && (GCC_VERSION < 40500) // test if GCC > 4.5.0 https://gcc.gnu.org/c99status.html
    #define STDINT_MISSING   1
    #endif
#elif defined(__APPLE__)
    #define PLATFORM_IOS	 1
    // macro define of TARGET_OS_IPHONE, TARGET_OS_SIMULATOR, TARGET_CPU_ARM, TARGET_CPU_ARM64 etc
    #include <TargetConditionals.h>
#elif defined(__linux__) || defined(__unix__) || defined(linux) || defined(__linux)
    #define PLATFORM_LINUX   1
#else
    #define PLATFORM_UNKNOWN 1
    #define STDINT_MISSING   1
#endif

// iOS Neon support, it's also can defined in xcode "Other C Flags" by separate arch
#define ENABLE_NEON_SUPPORT 1
#if (TARGET_OS_IPHONE != 0) && defined(ENABLE_NEON_SUPPORT)
    #if !defined(HAVE_NEON)
        #define HAVE_NEON
    #endif
    #if TARGET_CPU_ARM != 0
    #if !defined(HAVE_NEON32)
        #define HAVE_NEON32
    #endif
    #elif TARGET_CPU_ARM64 != 0
    #if !defined(HAVE_NEON64)
        #define HAVE_NEON64
    #endif
    #endif
#endif

// common includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <float.h>      // for FLT_EPSILON etc

using std::max;
using std::min;

#if defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#include <unistd.h>
#include <pthread.h>
#elif defined(PLATFORM_WINDOWS)
#else
#endif

#if defined(STDINT_MISSING)
#else
#include <stdint.h>
#endif

// BGRA sequence
#define MT_RED      0
#define MT_GREEN    1
#define MT_BLUE     2
#define MT_ALPHA    3

// OpenGL color space
#define GL_CHANNLE_R 0
#define GL_CHANNLE_G 1
#define GL_CHANNLE_B 2
#define GL_CHANNLE_A 3

// type define
typedef unsigned char       byte;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        UINT;
typedef unsigned int        uint32;
typedef long long           int64;
typedef unsigned long long  uint64;
typedef long                MTCOLORREF;
typedef float               REAL;

#undef TRUE
#define	TRUE                true
#undef FALSE
#define	FALSE               false

// safe delete
#undef SALFDELETE
#define SALFDELETE(x) if((x)!=NULL){ delete (x); (x)=NULL; }

#undef SAFE_DELETE
#define SAFE_DELETE(x) if((x)!=NULL){ delete (x); (x)=NULL; }

#undef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if((x)!=NULL){ delete[] (x); (x)=NULL; }


#endif
