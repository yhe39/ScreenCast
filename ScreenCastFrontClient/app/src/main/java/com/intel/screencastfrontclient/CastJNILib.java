package com.intel.screencastfrontclient;

public class CastJNILib {
    static {
        System.loadLibrary("CastFrontClientJni");
    }

    public static native String stringFromJNI();

    public static native void init();
    public static native void startCast();
    public static native void stopCast();
}

