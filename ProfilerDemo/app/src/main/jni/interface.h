//
// Created by 陈泽阳 on 2021/12/9.
//

#ifndef ProfilerDemo_INTERFACE_H
#define ProfilerDemo_INTERFACE_H

#ifdef __cplusplus
extern "C"{
#endif

/**
 * mode 取值范围 0，1，2
 * mode = 0 =》 行x行
 * mode = 1 =》 行x列
 * mode = 2 =》 列x列
 * return runtime ms
 * */
double test(int mode);

#ifdef __cplusplus
}
#endif

#endif //ProfilerDemo_INTERFACE_H
