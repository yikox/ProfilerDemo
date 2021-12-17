#include <iostream>
#include <string>

#include <GLES3/gl31.h>

#include "EGL/mtegl.h"
#include "glcompute.hpp"
#include "time_record.hpp"
#include "glcsShader.hpp"
#include "interface.h"
double test(int mode)
{
    time_record tm;

    EGLEnvionment m_egl_env;
    EGLUtil::egl_init_display(&m_egl_env, 32, 32);

    int c = 4;
    int h = 2048;
    int w = 2048;

    float * pMat = new float[c * h * w];
    for(int i = 0; i < c * h * w; i++)
        pMat[i] = (i % 100) / 50;

    GLuint InputA = GenDataBuffer(c * h * w * sizeof(float));
    GLuint InputB = GenDataBuffer(c * h * w * sizeof(float));
    GLuint Output = GenDataBuffer(c * h * w * sizeof(float));

    StoreBufferData(InputA, pMat, c * h * w * sizeof(float));
    StoreBufferData(InputB, pMat, c * h * w * sizeof(float));

    // 读取脚本
    std::string glCode;
    if(mode == 0)
        glCode = ((const char*)matmul_comp_cc);
    else if(mode == 1)
        glCode = ((const char*)matmul_comp_cr);
    else if(mode == 2)
        glCode = ((const char*)matmul_comp_rr);
    else
        printf("error mode = %d\n",mode);

    //编译脚本
    GLuint uId = CreateProgram(glCode.c_str());

    //输入参数
    SetBuffer(0, Output);
    SetBuffer(1, InputA);
    SetBuffer(2, InputB);

    //c=ab
    int iShapeA[4] = {2048,2048,4,1};
    int iShapeB[4] = {2048,2048,4,1};
    int iShapeC[4] = {2048,2048,4,1};
    SetArgs(3,iShapeA);
    SetArgs(4,iShapeB);
    SetArgs(5,iShapeC);

    int d0 = w/16;
    int d1 = h/16;
    int d2 = c/4;


    tm.record_begin();
    run(uId, d0, d1, d2);
    glFinish();
    tm.record_end();
    tm.record_printf("RunTime");
//    printf("Hello\n");
    return tm.record_getTimeMs();
}