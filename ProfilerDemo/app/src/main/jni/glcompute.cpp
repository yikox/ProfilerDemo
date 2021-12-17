#include <iostream>
#include "glcompute.hpp"



bool run(GLuint mId, int d0, int d1, int d2) {
    OPENGL_CHECK_ERROR
    glUseProgram(mId);
    OPENGL_CHECK_ERROR
    
    for (auto &a : args_) {
        glUniform4iv(a.first, 1, a.second);
    }
    OPENGL_CHECK_ERROR

    for (auto &b : buffers_) {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, b.first, b.second);
    }
    OPENGL_CHECK_ERROR

    glDispatchCompute(d0, d1, d2);

    OPENGL_CHECK_ERROR
    glFlush();

    return true;
}

void SetBuffer(int bindpoint, GLuint id) {
    buffers_[bindpoint] = id;
}
void SetArgs(int bindpoint, int *p) {
    args_[bindpoint] = p;
}
/**
 * CompileShader编译着色器脚本
 * shaderType = GL_COMPUTE_SHADER
*/
GLuint CompileShader(const char* pSource, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            // GetError(glGetShaderInfoLog, shader);
//            printf("CompileShader error\n");
            glDeleteShader(shader);
            shader = 0;
        }
    }
    OPENGL_CHECK_ERROR
    return shader;
}

GLuint CreateProgram(const char* pSource)
{
    GLuint programId = glCreateProgram();
    GLuint shaderID = CompileShader(pSource, GL_COMPUTE_SHADER);
    OPENGL_CHECK_ERROR
    glAttachShader(programId, shaderID);
    OPENGL_CHECK_ERROR
    glLinkProgram(programId);
    OPENGL_CHECK_ERROR
    GLint linked = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &linked);
    OPENGL_CHECK_ERROR
    if (!linked) {
		// GetError(glGetProgramInfoLog, programId);
//        printf("link error\n");
		glDeleteProgram(programId);
		programId = 0;
	}
	if(shaderID)
		glDeleteShader(shaderID);
    OPENGL_CHECK_ERROR
	return programId;
}

GLuint GenDataBuffer(int size)
{
    GLuint mId;
    GLASSERT(size > 0);
    glGenBuffers(1, &mId);
    OPENGL_CHECK_ERROR
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mId);
    OPENGL_CHECK_ERROR
    GLASSERT(mId > 0);
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    OPENGL_CHECK_ERROR
    return mId;
}

void DelDataBuffer(GLuint mId) {
    if (mId != GL_INVALID_INDEX) {
        glDeleteBuffers(1, &mId);
    } 
    OPENGL_CHECK_ERROR
}



void *map(GLuint mId, GLbitfield bufMask, GLsizeiptr mByteSize) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mId);
    OPENGL_CHECK_ERROR
    void* ptr = nullptr;

    ptr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, mByteSize, bufMask);
    OPENGL_CHECK_ERROR

    return ptr;
}

void unmap(GLuint mId) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mId);

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    OPENGL_CHECK_ERROR
}


bool StoreBufferData(GLuint mId, const void *ptr, int byte_size, int offset) {
    auto ret = true;

    uint8_t* buf = (uint8_t*)map(mId, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT, byte_size);
    if (buf != nullptr) {
        if (!offset){
            ::memset(buf, 0, byte_size);
        }
        ::memcpy(buf + offset, ptr, byte_size);  
    } else {
        ret = false;
    }
    unmap(mId);
    OPENGL_CHECK_ERROR
    return ret;
}

// bool Read(void *ptr, int byte_size ,FloatPrecision dst_precision) {
//     uint8_t* buf = (uint8_t*)map(GL_MAP_READ_BIT);
//     if (buf != nullptr) {
//         if( mPrecision == dst_precision ){ //当前浮点精度和目标浮点精度一致 ,无需转换
//             if( byte_size > mByteSize ){ 
//                 LOGE("Read too much : %d " , byte_size );
//             }
//             memcpy(ptr, buf, byte_size);
//         }else{
//             if( dst_precision == PRECISION_F32 ){// ptr为 fp32
//                 if( byte_size/sizeof(float) > mByteSize/mElementSize ){ 
//                     LOGE("Read too much : %lu " , byte_size/sizeof(float) );
//                 }
//                 convert_fp16_fp32(  (uint32_t*)buf ,(float*)ptr , byte_size/sizeof(float) );
//             }else if( dst_precision == PRECISION_FP16 ) { // ptr为 fp16
//                 if( byte_size/sizeof(uint16_t) > mByteSize/mElementSize ){ 
//                     LOGE("Read too much : %lu " , byte_size/sizeof(float) );
//                 }
//                 convert_fp32_fp16( (const float*)buf , (uint32_t*)ptr , byte_size/sizeof(uint16_t) );
//             }
//         }
//     }
//     unmap();
//     OPENGL_CHECK_ERROR
//     return buf != nullptr;
// }
const char* ErrorToString(int error) {
  switch (error) {
    case GL_INVALID_ENUM:
      return "[GL_INVALID_ENUM]: An unacceptable value is specified for an "
             "enumerated argument.";
    case GL_INVALID_VALUE:
      return "[GL_INVALID_VALUE]: A numeric argument is out of range.";
    case GL_INVALID_OPERATION:
      return "[GL_INVALID_OPERATION]: The specified operation is not allowed "
             "in the current state.";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      return "[GL_INVALID_FRAMEBUFFER_OPERATION]: The framebuffer object is "
             "not complete.";
    case GL_OUT_OF_MEMORY:
      return "[GL_OUT_OF_MEMORY]: There is not enough memory left to execute "
             "the command.";
  }
  return "[UNKNOWN_GL_ERROR]";
}
int GetOpenGlErrors() {
  auto error = glGetError();
  if (error == GL_NO_ERROR) {
    return GL_NO_ERROR;
  }
  printf("%s", ErrorToString(error));
  for (auto error2 = glGetError(); error2 != GL_NO_ERROR; error2 = glGetError()) {
    printf("%s", ErrorToString(error2));
  }
  return error;
}
