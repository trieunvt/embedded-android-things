package com.example.application.EmbeddedAndroidThings;

public class I2c {
    public static final int INDEX_0 = 0;
    public static final int INDEX_1 = 1;

    private int index;
    private int fileDescriptor;

    public I2c() {
    }

    public native boolean close();

    public native String getName();

    public native byte[] read(int length);

    public native byte readRegByte(int address);

    public native short readRegWord(int address);

    public native byte[] readRegBuffer(int startAddress, int length);

    public native boolean write(byte[] data);

    public native boolean writeRegByte(int address, byte value);

    public native boolean writeRegWord(int address, short value);

    public native boolean writeRegBuffer(int startAddress, byte[] data);

    static {
        System.loadLibrary("EmbeddedAndroidThings");
    }
}
