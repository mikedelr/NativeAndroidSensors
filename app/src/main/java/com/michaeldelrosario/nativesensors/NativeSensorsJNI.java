package com.michaeldelrosario.nativesensors;

public class NativeSensorsJNI {
    static {
        System.loadLibrary("nativesensors");
    }

    public static native void nativeFree();
    public static native void nativeInit();
    public static native void nativeStart();
    public static native void nativeStop();
}
