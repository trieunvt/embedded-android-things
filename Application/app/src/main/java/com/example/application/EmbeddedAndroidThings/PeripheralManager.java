package com.example.application.EmbeddedAndroidThings;

import java.util.ArrayList;
import java.util.List;

public class PeripheralManager {
    private static final PeripheralManager INSTANCE = new PeripheralManager();

    private PeripheralManager() {
    }

    public static PeripheralManager getInstance() {
        return INSTANCE;
    }

    private List<Integer> convertArrayToList(int[] array) {
        List<Integer> list = new ArrayList<>();
        for (int i : array) {
            list.add(i);
        }
        return list;
    }

    public List<Integer> getGpioList() {
        return convertArrayToList(getGpioListNative());
    }

    public List<Integer> getI2cList() {
        return convertArrayToList(getI2cListNative());
    }

    public List<Integer> getUartList() {
        return convertArrayToList(getUartListNative());
    }

    public Gpio openGpio(int chip, int line) {
        return openGpioNative(Gpio.getInstance(chip, line), chip, line);
    }

    public I2c openI2c(int index, int address) {
        return openI2cNative(new I2c(), index, address);
    }

    public Uart openUart(int index) {
        return openUartNative(new Uart(), index);
    }

    private native int[] getGpioListNative();

    private native int[] getI2cListNative();

    private native int[] getUartListNative();

    private native Gpio openGpioNative(Gpio gpio, int chip, int line);

    private native I2c openI2cNative(I2c i2c, int index, int address);

    private native Uart openUartNative(Uart uart, int index);

    static {
        System.loadLibrary("EmbeddedAndroidThings");
    }
}
