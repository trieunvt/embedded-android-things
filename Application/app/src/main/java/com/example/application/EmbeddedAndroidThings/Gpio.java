package com.example.application.EmbeddedAndroidThings;

public class Gpio {
    public static final int CHIP_A = 0;
    public static final int CHIP_B = 1;

    public static final int LINE_0 = 0;
    public static final int LINE_1 = 1;

    public static final int STATE_INVALID = -2;
    public static final int STATE_NOT_USED = -1;
    public static final int STATE_LOW = 0;
    public static final int STATE_HIGH = 1;

    public static final int DIRECTION_OUT = 0;
    public static final int DIRECTION_IN = 1;

    private static final Gpio INSTANCE_A0 = new Gpio();
    private static final Gpio INSTANCE_A1 = new Gpio();
    private static final Gpio INSTANCE_B0 = new Gpio();
    private static final Gpio INSTANCE_B1 = new Gpio();

    private int chip;
    private int line;
    private int state;
    private int direction;
    private int fileDescriptor;

    private Gpio() {
    }

    public static Gpio getInstance(int chip, int line) {
        if (chip == CHIP_A) {
            if (line == LINE_0)
                return INSTANCE_A0;

            if (line == LINE_1)
                return INSTANCE_A1;
        }

        if (chip == CHIP_B) {
            if (line == LINE_0)
                return INSTANCE_B0;

            if (line == LINE_1)
                return INSTANCE_B1;
        }

        return null;
    }

    public native boolean close();

    public native String getName();

    public native boolean setDirection(int direction);

    public native boolean setState(int state);

    public native int getState();

    static {
        System.loadLibrary("EmbeddedAndroidThings");
    }
}
