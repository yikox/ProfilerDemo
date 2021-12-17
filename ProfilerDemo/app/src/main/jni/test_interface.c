//
// Created by 陈泽阳 on 2021/12/9.
//
#include "interface.h"
#include <jni.h>
#include <stdio.h>
jdouble Java_com_example_ProfilerDemo_JNI_test0(JNIEnv *env, jobject jobj)
{
    return test(0);
}
jdouble Java_com_example_ProfilerDemo_JNI_test1(JNIEnv *env, jobject jobj)
{
    return test(1);
}
jdouble Java_com_example_ProfilerDemo_JNI_test2(JNIEnv *env, jobject jobj)
{
    return test(2);
}