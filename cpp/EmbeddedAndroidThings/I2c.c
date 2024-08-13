#include "Include/Essentials.h"
#include "Include/I2c.h"

const char *getI2cNameNative(JNIEnv *env, jobject obj) {
    jstring jName = JNICREATE(I2c, getName)(env, obj);
    const char *name = (*env)->GetStringUTFChars(env, jName, 0);
    (*env)->ReleaseStringUTFChars(env, jName, name);
    return name;
}

int openI2cNative(int index, int address) {
    char device[32];
    sprintf(device, "%s%d", I2C_NAME, index);

    int fileDescriptor = open(device, O_RDWR);
    if (0 > fileDescriptor) {
        __android_log_print(ANDROID_LOG_ERROR, I2C_TAG, "Failed to open %s", device);
        return fileDescriptor;
    }

    if (0 > ioctl(fileDescriptor, I2C_SLAVE, address)) {
        __android_log_print(ANDROID_LOG_ERROR, I2C_TAG, "Failed to open %s", device);
        return -2;
    }

    return fileDescriptor;
}

JNIEXPORT jboolean JNICALL
JNICREATE(I2c, close)
        (JNIEnv *env, jobject obj) {
    jint fileDescriptor = getIntFieldNative(env, obj, PACKAGE_PATH"/I2c", "fileDescriptor");
    if (0 > close(fileDescriptor)) {
        __android_log_print(ANDROID_LOG_ERROR, I2C_TAG, "Failed to close %s",
                            getI2cNameNative(env, obj));
        return false;
    } else return true;
}

JNIEXPORT jstring JNICALL
JNICREATE(I2c, getName)
        (JNIEnv *env, jobject obj) {
    char name[32];
    jint index = getIntFieldNative(env, obj, PACKAGE_PATH"/I2c", "index");
    sprintf(name, "%s%d", I2C_NAME, index);
    return (*env)->NewStringUTF(env, name);
}

JNIEXPORT jbyteArray JNICALL
JNICREATE(I2c, read)
        (JNIEnv *env, jobject obj, jint length) {
    return JNICREATE(I2c, readRegBuffer)(env, obj, 0, length);
}

JNIEXPORT jbyte JNICALL
JNICREATE(I2c, readRegByte)
        (JNIEnv *env, jobject obj, jint address) {
    jbyteArray data = JNICREATE(I2c, readRegBuffer)(env, obj, address, 1);
    jbyte *pData = (*env)->GetByteArrayElements(env, data, NULL);
    jbyte result = pData[0];
    (*env)->ReleaseByteArrayElements(env, data, pData, 0);
    return result;
}

JNIEXPORT jshort JNICALL
JNICREATE(I2c, readRegWord)
        (JNIEnv *env, jobject obj, jint address) {
    jbyteArray data = JNICREATE(I2c, readRegBuffer)(env, obj, address, 2);
    jbyte *pData = (*env)->GetByteArrayElements(env, data, NULL);
    jshort result = (pData[0] << 8) + pData[1];
    (*env)->ReleaseByteArrayElements(env, data, pData, 0);
    return result;
}

JNIEXPORT jbyteArray JNICALL
JNICREATE(I2c, readRegBuffer)
        (JNIEnv *env, jobject obj, jint startAddress, jint length) {
    if (0 == length) {
        /* Prevent crash Android App */
        jbyte buffer[1] = "\0";
        jbyteArray data = (*env)->NewByteArray(env, 1);
        (*env)->SetByteArrayRegion(env, data, 0, 1, buffer);
        return data;
    }

    jint fileDescriptor = getIntFieldNative(env, obj, PACKAGE_PATH"/I2c", "fileDescriptor");
    jbyte buffer[length];

    if (0 > write(fileDescriptor, &startAddress, 1)) {
        memset(buffer, 0, length);
        __android_log_print(ANDROID_LOG_ERROR, I2C_TAG,
                            "Failed to write register address 0x%02x to %s", startAddress,
                            getI2cNameNative(env, obj));
    } else if (0 > read(fileDescriptor, buffer, length)) {
        memset(buffer, 0, length);
        __android_log_print(ANDROID_LOG_ERROR, I2C_TAG, "Failed to read data from %s",
                            getI2cNameNative(env, obj));
    }

    jbyteArray data = (*env)->NewByteArray(env, length);
    (*env)->SetByteArrayRegion(env, data, 0, length, buffer);
    return data;
}

JNIEXPORT jboolean JNICALL
JNICREATE(I2c, write)
        (JNIEnv *env, jobject obj, jbyteArray data) {
    return JNICREATE(I2c, writeRegBuffer)(env, obj, 0, data);
}

JNIEXPORT jboolean JNICALL
JNICREATE(I2c, writeRegByte)
        (JNIEnv *env, jobject obj, jint address, jbyte value) {
    jbyteArray data = (*env)->NewByteArray(env, 1);
    jbyte *pData = (*env)->GetByteArrayElements(env, data, NULL);
    pData[0] = value;
    (*env)->ReleaseByteArrayElements(env, data, pData, 0);
    return JNICREATE(I2c, writeRegBuffer)(env, obj, address, data);
}

JNIEXPORT jboolean JNICALL
JNICREATE(I2c, writeRegWord)
        (JNIEnv *env, jobject obj, jint address, jshort value) {
    jbyteArray data = (*env)->NewByteArray(env, 2);
    jbyte *pData = (*env)->GetByteArrayElements(env, data, NULL);
    pData[0] = value >> 8;
    pData[1] = value & 0xFF;
    (*env)->ReleaseByteArrayElements(env, data, pData, 0);
    return JNICREATE(I2c, writeRegBuffer)(env, obj, address, data);
}

JNIEXPORT jboolean JNICALL
JNICREATE(I2c, writeRegBuffer)
        (JNIEnv *env, jobject obj, jint startAddress, jbyteArray data) {
    jint fileDescriptor = getIntFieldNative(env, obj, PACKAGE_PATH"/I2c", "fileDescriptor");
    jsize length = (*env)->GetArrayLength(env, data);
    jbyte *pData = (*env)->GetByteArrayElements(env, data, NULL);
    uint8_t transmitData[length + 1];

    transmitData[0] = startAddress;
    for (int i = 0; i < length; ++i) {
        transmitData[i + 1] = pData[i];
    }

    for (int i = 0; i < length + 1; ++i) {
        __android_log_print(ANDROID_LOG_DEBUG, I2C_TAG, "data[%d] = 0x%02x", i, transmitData[i]);
    }

    if (0 > write(fileDescriptor, transmitData, length + 1)) {
        __android_log_print(ANDROID_LOG_ERROR, I2C_TAG, "Failed to write data to %s",
                            getI2cNameNative(env, obj));
        return false;
    } else return true;
}
