package com.example.ProfilerDemo;

public class JNI {
    {
        // 在这里我们需要加载这个jni so库， 这个Hello名字就是最终编译产出的so的名字，也可以起其他的名字，但必须要和最终的so库名相同。
        System.loadLibrary("gldemo");
    }
    /* *
     *  定义native方法
     *  调用C代码对应的方法
     *  @return
     */
    public native double test0();
    public native double test1();
    public native double test2();
}
