#pragma once

#include <unordered_map>
#include <GLES3/gl31.h>

int GetOpenGlErrors();

#define OPENGL_CHECK_ERROR              \
    {                                   \
        GLenum error = GetOpenGlErrors();    \
        GLASSERT(GL_NO_ERROR == error); \
    }

// #define OPENGL_CHECK_ERROR 

#define GLASSERT(x) assert(x)

#ifndef GL_SHADER_STORAGE_BUFFER
    #define GL_SHADER_STORAGE_BUFFER 0x90D2
#endif

#ifndef GL_DYNAMIC_DRAW
    #define GL_DYNAMIC_DRAW 0x88E8
#endif

static std::unordered_map<int, int> buffers_;
static std::unordered_map<int, int*> args_;

// GLuint CompileShader(const char* pSource, GLenum shaderType=GL_COMPUTE_SHADER);
GLuint CreateProgram(const char* pSource);

void SetBuffer(int bindpoint, GLuint id);

void SetArgs(int bindpoint, int *p);

bool run(GLuint mId,int d0, int d1, int d2);

GLuint GenDataBuffer(int size);

void DelDataBuffer(GLuint mId);

bool StoreBufferData(GLuint mId, const void *ptr, int byte_size, int offset = 0);

// void* map(GLbitfield bufMask);

// void unmap();
