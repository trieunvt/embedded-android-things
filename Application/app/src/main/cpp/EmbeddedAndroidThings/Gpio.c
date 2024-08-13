#include "Include/Gpio.h"
#include "Include/Essentials.h"

const char *getGpioNameNative(JNIEnv *env, jobject obj) {
    jstring jName = JNICREATE(Gpio, getName)(env, obj);
    const char *name = (*env)->GetStringUTFChars(env, jName, 0);
    (*env)->ReleaseStringUTFChars(env, jName, name);
    return name;
}

bool openGpioNative(JNIEnv *env, jobject gpio, jint chip, jint line) {
    if (0 < getIntFieldNative(env, gpio, PACKAGE_PATH"/Gpio", "fileDescriptor")) return true;

    if (JNICREATE(Gpio, setDirection)(env, gpio, JNIDEFINE(Gpio, DIRECTION_OUT))) {
        if (JNICREATE(Gpio, setState)(env, gpio, JNIDEFINE(Gpio, STATE_LOW))) {
            return true;
        }
    }

    __android_log_print(ANDROID_LOG_ERROR, GPIO_TAG, "Failed to open %s",
                        getGpioNameNative(env, gpio));
    return false;
}

JNIEXPORT jboolean JNICALL
JNICREATE(Gpio, close)
        (JNIEnv *env, jobject obj) {
    if (JNIDEFINE(Gpio, STATE_NOT_USED) ==
        getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "state")) return true;

    jint fileDescriptor = getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "fileDescriptor");
    if (0 > close(fileDescriptor)) {
        __android_log_print(ANDROID_LOG_ERROR, GPIO_TAG, "Failed to close %s",
                            getGpioNameNative(env, obj));
        return false;
    }

    setIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "state", JNIDEFINE(Gpio, STATE_NOT_USED));
    return true;
}

JNIEXPORT jstring JNICALL
JNICREATE(Gpio, getName)
        (JNIEnv *env, jobject obj) {
    jint chip = getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "chip");
    jint line = getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "line");
    char name[32];
    sprintf(name, "%s%d-%s%d", GPIO_CHIP_NAME, chip, GPIO_LINE_NAME, line);
    return (*env)->NewStringUTF(env, name);
}

JNIEXPORT jboolean JNICALL
JNICREATE(Gpio, setDirection)
        (JNIEnv *env, jobject obj, jint direction) {
    char device[32];
    jint chip = getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "chip");
    jint line = getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "line");
    sprintf(device, "%s%d", GPIO_CHIP_NAME, chip);

    /* Close previous direction file descriptor before changing, allow init failure */
    close(getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "fileDescriptor"));

    int fileDescriptor = open(device, O_RDWR | O_CLOEXEC);
    if (0 > fileDescriptor) {
        __android_log_print(ANDROID_LOG_ERROR, GPIO_TAG,
                            "Failed to open to set direction of %s", device);
        return false;
    }

    struct gpiohandle_request request;
    request.lineoffsets[0] = line;
    request.lines = 1;
    if (JNIDEFINE(Gpio, DIRECTION_IN) == direction) {
        request.flags = GPIOHANDLE_REQUEST_INPUT;
    } else if (JNIDEFINE(Gpio, DIRECTION_OUT) == direction) {
        request.flags = GPIOHANDLE_REQUEST_OUTPUT;
    } else {
        __android_log_print(ANDROID_LOG_ERROR, GPIO_TAG, "Invalid Gpio direction");
        return false;
    }

    int result = ioctl(fileDescriptor, GPIO_GET_LINEHANDLE_IOCTL, &request);
    if (0 > close(fileDescriptor)) {
        __android_log_print(ANDROID_LOG_ERROR, GPIO_TAG,
                            "Failed to close after setting direction of %s",
                            getGpioNameNative(env, obj));
//        return false;
    }

    if (0 > result) {
        __android_log_print(ANDROID_LOG_ERROR, GPIO_TAG, "Failed to set direction of %s",
                            getGpioNameNative(env, obj));
        return false;
    } else {
        setIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "fileDescriptor", request.fd);
        setIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "direction", direction);
        return true;
    }
}

JNIEXPORT jboolean JNICALL
JNICREATE(Gpio, setState)
        (JNIEnv *env, jobject obj, jint state) {
    if (JNIDEFINE(Gpio, DIRECTION_OUT) !=
        getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "direction")) {
        __android_log_print(ANDROID_LOG_ERROR, GPIO_TAG, "Not Gpio output: %s",
                            getGpioNameNative(env, obj));
        return false;
    }

    if (0 > state) {
        __android_log_print(ANDROID_LOG_ERROR, GPIO_TAG, "Failed to set state of %s to negative state",
                            getGpioNameNative(env, obj));
        return false;
    }

    jint fileDescriptor = getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "fileDescriptor");
    struct gpiohandle_data data;
    data.values[0] = state;
    if (0 > ioctl(fileDescriptor, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data)) {
        __android_log_print(ANDROID_LOG_ERROR, GPIO_TAG, "Failed to set state of %s",
                            getGpioNameNative(env, obj));
        return false;
    }

    setIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "state", state);
    return true;
}

JNIEXPORT jint JNICALL
JNICREATE(Gpio, getState)
        (JNIEnv *env, jobject obj) {
    if (JNIDEFINE(Gpio, DIRECTION_OUT) ==
        getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "direction"))
        return getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "state");

    if (JNIDEFINE(Gpio, DIRECTION_IN) !=
        getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "direction")) {
        __android_log_print(ANDROID_LOG_ERROR, GPIO_TAG, "Direction of %s invalid",
                            getGpioNameNative(env, obj));
        return JNIDEFINE(Gpio, STATE_INVALID);
    }

    jint fileDescriptor = getIntFieldNative(env, obj, PACKAGE_PATH"/Gpio", "fileDescriptor");
    struct gpiohandle_data data;
    if (0 > ioctl(fileDescriptor, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data)) {
        __android_log_print(ANDROID_LOG_ERROR, GPIO_TAG, "Failed to get state of %s",
                            getGpioNameNative(env, obj));
        return JNIDEFINE(Gpio, STATE_INVALID);
    } else return data.values[0];
}
