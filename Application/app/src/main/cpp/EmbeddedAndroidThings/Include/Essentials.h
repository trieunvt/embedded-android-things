#ifndef ESSENTIALS_H
#define ESSENTIALS_H

#include <jni.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <android/log.h>
#include <linux/gpio.h>
#include <linux/i2c-dev.h>

#define JNIDEFINE(jniClass, jniMacro) \
        com_example_application_EmbeddedAndroidThings##_##jniClass##_##jniMacro

#define JNICREATE(jniClass, jniMethod) \
        Java_##com_example_application_EmbeddedAndroidThings##_##jniClass##_##jniMethod

#define PACKAGE_PATH "com/example/application/EmbeddedAndroidThings"

#define PERIPHERAL_TAG  "PeripheralManager"

#define GPIO_TAG        "Gpio"
#define GPIO_CHIP_NAME  "/dev/gpiochip"
#define GPIO_LINE_NAME  "line"
#define GPIO_CHIP_LIMIT 5

#define I2C_TAG         "I2c"
#define I2C_NAME        "/dev/i2c-"
#define I2C_LIMIT       5

#define UART_TAG        "Uart"
#define UART_NAME       "/dev/ttyHS"
#define DEFAULT_BAUDRATE B9600
#define UART_LIMIT      5

bool openGpioNative(JNIEnv *env, jobject gpio, jint chip, jint line);
int openI2cNative(int index, int address);
int openUartNative(int index);
jint getIntFieldNative
    (JNIEnv *env, jobject obj, const char *classpath, const char *field);
void setIntFieldNative
    (JNIEnv *env, jobject obj, const char *classpath, const char *field, jint value);

#endif /* ESSENTIALS_H */
