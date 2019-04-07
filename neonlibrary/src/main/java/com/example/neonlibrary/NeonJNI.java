package com.example.neonlibrary;

import java.nio.ByteBuffer;

/**
 * Created by Hanlon on 2019/4/5.
 */

public class NeonJNI {

    static{
        System.loadLibrary("myNeonJNI");
    }

    static public native void runNeon(ByteBuffer in, int inSize, ByteBuffer out , int outSize);
    static public native void runNormal(ByteBuffer in,int inSize, ByteBuffer out, int outSize);

    static public native boolean neonMul(float[] a , float[] b, float[] result);
}
