#include "Include/Essentials.h"
#include "Include/PeripheralManager.h"

JNIEXPORT jintArray JNICALL
JNICREATE(PeripheralManager, getGpioListNative)
        (JNIEnv *env, jobject obj) {
    char device[32];
    struct gpiochip_info chipInfo;
    struct gpioline_info lineInfo;
    jintArray intArray = (*env)->NewIntArray(env, 2 * GPIO_CHIP_LIMIT);
    jint *array = (*env)->GetIntArrayElements(env, intArray, NULL);
    for (int chip = 0; chip < GPIO_CHIP_LIMIT; ++chip) {
        sprintf(device, "%s%d", GPIO_CHIP_NAME, chip);
        int fileDescriptor = open(device, O_RDONLY);

        if (0 > fileDescriptor) {
            __android_log_print(ANDROID_LOG_INFO, PERIPHERAL_TAG,
                                "Failed to open to read chip info of %s", device);
            continue;
        }

        if (0 > ioctl(fileDescriptor, GPIO_GET_CHIPINFO_IOCTL, &chipInfo)) {
            __android_log_print(ANDROID_LOG_INFO, PERIPHERAL_TAG,
                                "Failed to ioctl to read chip info of %s", device);
            close(fileDescriptor);
            continue;
        }

        array[2 * chip] = chip;
        array[2 * chip + 1] = chipInfo.lines;
        __android_log_print(ANDROID_LOG_INFO, PERIPHERAL_TAG,
                            "Chip name: %s", chipInfo.name);
        __android_log_print(ANDROID_LOG_INFO, PERIPHERAL_TAG,
                            "Chip label: %s", chipInfo.label);
        __android_log_print(ANDROID_LOG_INFO, PERIPHERAL_TAG,
                            "Number of lines: %d", chipInfo.lines);

        for (int line = 0; line < chipInfo.lines; ++line) {
            lineInfo.line_offset = line;
            if (0 > ioctl(fileDescriptor, GPIO_GET_LINEINFO_IOCTL, &lineInfo)) {
                __android_log_print(ANDROID_LOG_INFO, PERIPHERAL_TAG,
                                    "Failed to read info of line %d of %s", line, device);
            } else {
                __android_log_print(ANDROID_LOG_INFO, PERIPHERAL_TAG,
                                    "Offset: %d, Name: %s, Consumer: %s | Flags: [%s] [%s] [%s] [%s] [%s]",
                                    line,
                                    lineInfo.name,
                                    lineInfo.consumer,
                                    (lineInfo.flags & GPIOLINE_FLAG_IS_OUT) ? "OUTPUT" : "INPUT",
                                    (lineInfo.flags & GPIOLINE_FLAG_ACTIVE_LOW) ? "ACTIVE_LOW"
                                                                                : "ACTIVE_HIGH",
                                    (lineInfo.flags & GPIOLINE_FLAG_OPEN_DRAIN) ? "OPEN_DRAIN"
                                                                                : "...",
                                    (lineInfo.flags & GPIOLINE_FLAG_OPEN_SOURCE) ? "OPENSOURCE"
                                                                                 : "...",
                                    (lineInfo.flags & GPIOLINE_FLAG_KERNEL) ? "KERNEL" : "...");
            }
        }
        close(fileDescriptor);
    }
    (*env)->ReleaseIntArrayElements(env, intArray, array, 0);
    return intArray;
}

JNIEXPORT jintArray JNICALL
JNICREATE(PeripheralManager, getI2cListNative)
        (JNIEnv *env, jobject obj) {
    char device[32];
    jintArray intArray = (*env)->NewIntArray(env, I2C_LIMIT);
    jint *array = (*env)->GetIntArrayElements(env, intArray, NULL);
    int index = 0;
    for (int dev = 0; dev < I2C_LIMIT; ++dev) {
        sprintf(device, "%s%d", I2C_NAME, dev);

        if (0 > open(device, O_RDONLY)) {
            __android_log_print(ANDROID_LOG_INFO, PERIPHERAL_TAG,
                                "Unable to access %s", device);
            array[index] = -1;
        } else {
            array[index] = dev;
        }
        index++;
    }
    (*env)->ReleaseIntArrayElements(env, intArray, array, 0);
    return intArray;
}

JNIEXPORT jintArray JNICALL
JNICREATE(PeripheralManager, getUartListNative)
        (JNIEnv *env, jobject obj) {
    char device[32];
    jintArray intArray = (*env)->NewIntArray(env, UART_LIMIT);
    jint *array = (*env)->GetIntArrayElements(env, intArray, NULL);
    int index = 0;
    for (int dev = 0; dev < UART_LIMIT; ++dev) {
        sprintf(device, "%s%d", UART_NAME, dev);

        if (0 > open(device, O_RDONLY)) {
            __android_log_print(ANDROID_LOG_INFO, PERIPHERAL_TAG,
                                "Unable to access %s", device);
            array[index] = -1;
        } else {
            array[index] = dev;
        }
        index++;
    }
    (*env)->ReleaseIntArrayElements(env, intArray, array, 0);
    return intArray;
}



JNIEXPORT jobject JNICALL
JNICREATE(PeripheralManager, openGpioNative)
        (JNIEnv *env, jobject obj, jobject gpio, jint chip, jint line) {
    if (NULL == gpio) {
        __android_log_print(ANDROID_LOG_ERROR, GPIO_TAG,
                            "Instance of %s%d-%s%d not found", GPIO_CHIP_NAME, chip, GPIO_LINE_NAME, line);
        return NULL;
    }

    setIntFieldNative(env, gpio, PACKAGE_PATH"/Gpio", "chip", chip);
    setIntFieldNative(env, gpio, PACKAGE_PATH"/Gpio", "line", line);
    if (openGpioNative(env, gpio, chip, line)) {
        return gpio;
    } else return NULL;
}

JNIEXPORT jobject JNICALL
JNICREATE(PeripheralManager, openI2cNative)
        (JNIEnv *env, jobject obj, jobject i2c, jint index, jint address) {
    setIntFieldNative(env, i2c, PACKAGE_PATH"/I2c", "index", index);
    int fileDescriptor = openI2cNative(index, address);
    if (0 > fileDescriptor) {
        return NULL;
    } else {
        setIntFieldNative(env, i2c, PACKAGE_PATH"/I2c",
                          "fileDescriptor", fileDescriptor);
        return i2c;
    }
}

JNIEXPORT jobject JNICALL
JNICREATE(PeripheralManager, openUartNative)
        (JNIEnv *env, jobject obj, jobject uart, jint index) {
    setIntFieldNative(env, uart, PACKAGE_PATH"/Uart", "index", index);
    int fileDescriptor = openUartNative(index);
    if (0 > fileDescriptor) {
        return NULL;
    } else {
        setIntFieldNative(env, uart, PACKAGE_PATH"/Uart",
                          "fileDescriptor", fileDescriptor);
        return uart;
    }
}
