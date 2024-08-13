package com.example.application.EmbeddedAndroidThings;

public class Uart {
    public static final int INDEX_0 = 0;
    public static final int INDEX_1 = 1;

    public static final int BAUDRATE_1200 = 1200;
    public static final int BAUDRATE_2400 = 2400;
    public static final int BAUDRATE_4800 = 4800;
    public static final int BAUDRATE_9600 = 9600;
    public static final int BAUDRATE_19200 = 19200;
    public static final int BAUDRATE_38400 = 38400;
    public static final int BAUDRATE_57600 = 57600;
    public static final int BAUDRATE_115200 = 115200;

    public static final int FLUSH_IN = 0;
    public static final int FLUSH_OUT = 1;
    public static final int FLUSH_IN_OUT = 2;

    private int index;
    private int fileDescriptor;

    public Uart() {
    }

    public native boolean close();

    public native String getName();

    public native boolean setBaudrate(int baudrate);

    public native int isDataAvailable();

    public native byte[] read(int length);

    public native boolean write(byte[] data);

    public native boolean flush(int type);

    static {
        System.loadLibrary("EmbeddedAndroidThings");
    }
}
