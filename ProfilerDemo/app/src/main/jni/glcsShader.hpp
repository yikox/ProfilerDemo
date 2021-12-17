#ifndef __GLCS_SHADER__
#define __GLCS_SHADER__

const  char* matmul_comp_cc = (const  char*)\
R"(#version 310 es
#define PRECISION highp
precision PRECISION float;
#define FORMAT rgba32f
#define WFORMAT rgba32f
#define XLOCAL 8
#define YLOCAL 8
#define ZLOCAL 1

layout(binding = 0) writeonly buffer soutput{
    vec4 data[];
} uOutput;
layout(binding = 1) readonly buffer sinput0{
    vec4 data[];
} uInput0;
layout(binding = 2) readonly buffer sinput1{
    vec4 data[];
} uInput1;

layout(location=3) uniform ivec4 uInputSize0;
layout(location=4) uniform ivec4 uInputSize1;
layout(location=5) uniform ivec4 uOutputSize;

layout (local_size_x = XLOCAL, local_size_y = YLOCAL, local_size_z = ZLOCAL) in;

//矩阵A和矩阵B相乘的某一列的第I个元素
vec4 PixelMul(int i, ivec3 pos)
{
    // 行x行
     vec4 data0 = uInput0.data[i + pos.y * uInputSize0.x + pos.z * uInputSize0.x * uInputSize0.y];
     vec4 data1 = uInput1.data[i + pos.x * uInputSize1.y + pos.z * uInputSize1.x * uInputSize1.y];

    return data0 * data1;
}
void main()
{
    ivec3 pos = ivec3(gl_GlobalInvocationID) * ivec3(2, 2, 1);
    if(all(lessThan(pos, uOutputSize.xyz)))
    {
        vec4 outData00 = vec4(0);
        vec4 outData01 = vec4(0);
        vec4 outData10 = vec4(0);
        vec4 outData11 = vec4(0);

        for(int i = 0; i < uInputSize0.x; i++)
        {
            outData00 += PixelMul(i, pos + ivec3(0, 0, 0));
            outData01 += PixelMul(i, pos + ivec3(1, 0, 0));
            outData10 += PixelMul(i, pos + ivec3(0, 1, 0));
            outData11 += PixelMul(i, pos + ivec3(1, 1, 0));
        }

        uOutput.data[pos.x + 0 + (pos.y + 0) * uOutputSize.x + pos.z * uOutputSize.x * uOutputSize.y] = outData00;
        uOutput.data[pos.x + 1 + (pos.y + 0) * uOutputSize.x + pos.z * uOutputSize.x * uOutputSize.y] = outData01;
        uOutput.data[pos.x + 0 + (pos.y + 1) * uOutputSize.x + pos.z * uOutputSize.x * uOutputSize.y] = outData10;
        uOutput.data[pos.x + 1 + (pos.y + 1) * uOutputSize.x + pos.z * uOutputSize.x * uOutputSize.y] = outData11;
    }
}

)";

const  char* matmul_comp_cr = (const  char*)\
R"(#version 310 es
#define PRECISION highp
precision PRECISION float;
#define FORMAT rgba32f
#define WFORMAT rgba32f
#define XLOCAL 8
#define YLOCAL 8
#define ZLOCAL 1

layout(binding = 0) writeonly buffer soutput{
    vec4 data[];
} uOutput;
layout(binding = 1) readonly buffer sinput0{
    vec4 data[];
} uInput0;
layout(binding = 2) readonly buffer sinput1{
    vec4 data[];
} uInput1;

layout(location=3) uniform ivec4 uInputSize0;
layout(location=4) uniform ivec4 uInputSize1;
layout(location=5) uniform ivec4 uOutputSize;

layout (local_size_x = XLOCAL, local_size_y = YLOCAL, local_size_z = ZLOCAL) in;

//矩阵A和矩阵B相乘的某一列的第I个元素
vec4 PixelMul(int i, ivec3 pos)
{
    // 行x列
     vec4 data0 = uInput0.data[i + pos.y * uInputSize0.x + pos.z * uInputSize0.x * uInputSize0.y];
     vec4 data1 = uInput1.data[pos.x + i * uInputSize1.y + pos.z * uInputSize1.x * uInputSize1.y];

    return data0 * data1;
}
void main()
{
    ivec3 pos = ivec3(gl_GlobalInvocationID) * ivec3(2, 2, 1);
    if(all(lessThan(pos, uOutputSize.xyz)))
    {
        vec4 outData00 = vec4(0);
        vec4 outData01 = vec4(0);
        vec4 outData10 = vec4(0);
        vec4 outData11 = vec4(0);

        for(int i = 0; i < uInputSize0.x; i++)
        {
            outData00 += PixelMul(i, pos + ivec3(0, 0, 0));
            outData01 += PixelMul(i, pos + ivec3(1, 0, 0));
            outData10 += PixelMul(i, pos + ivec3(0, 1, 0));
            outData11 += PixelMul(i, pos + ivec3(1, 1, 0));
        }

        uOutput.data[pos.x + 0 + (pos.y + 0) * uOutputSize.x + pos.z * uOutputSize.x * uOutputSize.y] = outData00;
        uOutput.data[pos.x + 1 + (pos.y + 0) * uOutputSize.x + pos.z * uOutputSize.x * uOutputSize.y] = outData01;
        uOutput.data[pos.x + 0 + (pos.y + 1) * uOutputSize.x + pos.z * uOutputSize.x * uOutputSize.y] = outData10;
        uOutput.data[pos.x + 1 + (pos.y + 1) * uOutputSize.x + pos.z * uOutputSize.x * uOutputSize.y] = outData11;
    }
}

)";


const  char* matmul_comp_rr = (const  char*)\
R"(#version 310 es
#define PRECISION highp
precision PRECISION float;
#define FORMAT rgba32f
#define WFORMAT rgba32f
#define XLOCAL 8
#define YLOCAL 8
#define ZLOCAL 1

layout(binding = 0) writeonly buffer soutput{
    vec4 data[];
} uOutput;
layout(binding = 1) readonly buffer sinput0{
    vec4 data[];
} uInput0;
layout(binding = 2) readonly buffer sinput1{
    vec4 data[];
} uInput1;

layout(location=3) uniform ivec4 uInputSize0;
layout(location=4) uniform ivec4 uInputSize1;
layout(location=5) uniform ivec4 uOutputSize;

layout (local_size_x = XLOCAL, local_size_y = YLOCAL, local_size_z = ZLOCAL) in;

//矩阵A和矩阵B相乘的某一列的第I个元素
vec4 PixelMul(int i, ivec3 pos)
{
    // 列x列
    vec4 data0 = uInput0.data[pos.y + i * uInputSize0.x + pos.z * uInputSize0.x * uInputSize0.y];
    vec4 data1 = uInput1.data[pos.x + i * uInputSize1.y + pos.z * uInputSize1.x * uInputSize1.y];

    return data0 * data1;
}
void main()
{
    ivec3 pos = ivec3(gl_GlobalInvocationID) * ivec3(2, 2, 1);
    if(all(lessThan(pos, uOutputSize.xyz)))
    {
        vec4 outData00 = vec4(0);
        vec4 outData01 = vec4(0);
        vec4 outData10 = vec4(0);
        vec4 outData11 = vec4(0);

        for(int i = 0; i < uInputSize0.x; i++)
        {
            outData00 += PixelMul(i, pos + ivec3(0, 0, 0));
            outData01 += PixelMul(i, pos + ivec3(1, 0, 0));
            outData10 += PixelMul(i, pos + ivec3(0, 1, 0));
            outData11 += PixelMul(i, pos + ivec3(1, 1, 0));
        }

        uOutput.data[pos.x + 0 + (pos.y + 0) * uOutputSize.x + pos.z * uOutputSize.x * uOutputSize.y] = outData00;
        uOutput.data[pos.x + 1 + (pos.y + 0) * uOutputSize.x + pos.z * uOutputSize.x * uOutputSize.y] = outData01;
        uOutput.data[pos.x + 0 + (pos.y + 1) * uOutputSize.x + pos.z * uOutputSize.x * uOutputSize.y] = outData10;
        uOutput.data[pos.x + 1 + (pos.y + 1) * uOutputSize.x + pos.z * uOutputSize.x * uOutputSize.y] = outData11;
    }
}

)";


#endif