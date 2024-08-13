#include "Include/Essentials.h"
#include "Include/Uart.h"

const char *getUartNameNative(JNIEnv *env, jobject obj) {
    jstring jName = JNICREATE(Uart, getName)(env, obj);
    const char *name = (*env)->GetStringUTFChars(env, jName, 0);
    (*env)->ReleaseStringUTFChars(env, jName, name);
    return name;
}

int openUartNative(int index) {
    char device[strlen(UART_NAME) + 3];
    sprintf(device, "%s%d", UART_NAME, index);

    int fileDescriptor = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
    if (0 > fileDescriptor) {
        __android_log_print(ANDROID_LOG_ERROR, UART_TAG, "Failed to open %s", device);
        return fileDescriptor;
    }
    /* Get and modify current option */
    struct termios option;
    fcntl(fileDescriptor, F_SETFL, O_RDWR);
    if (0 > tcgetattr(fileDescriptor, &option)) {
        __android_log_print(ANDROID_LOG_ERROR, UART_TAG, "Failed to open %s", device);
        return -2;
    }

    cfmakeraw(&option);
    cfsetspeed(&option, DEFAULT_BAUDRATE);


    option.c_cflag |= (CLOCAL | CREAD);     /* Ignore Modem Control lines, enable receiver */
    option.c_cflag &= ~PARENB;              /* Disable Parity Enable Bit */
    option.c_cflag &= ~CSTOPB;              /* Use 1 Stop Bit */
    option.c_cflag &= ~CSIZE;               /* Clear data size setting mask */
    option.c_cflag |= CS8;                  /* Set data bits = 8 */

    option.c_lflag &= ~(ICANON | ECHO | ECHONL | IEXTEN | ECHOE | ISIG);  /* Non-canonical mode */

    option.c_oflag &= ~OPOST;               /* No output processing */

    option.c_cc[VMIN] = 0;                  /* Read at least 0 characters */
    option.c_cc[VTIME] = 100;               /* Wait 100 seconds */

    if (0 > tcsetattr(fileDescriptor, TCSANOW, &option)) {
        __android_log_print(ANDROID_LOG_ERROR, UART_TAG, "Failed to open %s", device);
        return -3;
    }

    int status;
    ioctl(fileDescriptor, TIOCMGET, &status);
    status |= TIOCM_DTR;
    status |= TIOCM_RTS;
    ioctl(fileDescriptor, TIOCMSET, &status);

    return fileDescriptor;
}

JNIEXPORT jboolean JNICALL
JNICREATE(Uart, close)
        (JNIEnv *env, jobject obj) {
    jint fileDescriptor = getIntFieldNative(env, obj, PACKAGE_PATH"/Uart", "fileDescriptor");
    if (0 > close(fileDescriptor)) {
        __android_log_print(ANDROID_LOG_ERROR, UART_TAG, "Failed to close %s",
                            getUartNameNative(env, obj));
        return false;
    } else return true;
}

JNIEXPORT jstring JNICALL
JNICREATE(Uart, getName)
        (JNIEnv *env, jobject obj) {
    char name[32];
    jint index = getIntFieldNative(env, obj, PACKAGE_PATH"/Uart", "index");
    sprintf(name, "%s%d", UART_NAME, index);
    return (*env)->NewStringUTF(env, name);
}

JNIEXPORT jboolean JNICALL
JNICREATE(Uart, setBaudrate)
        (JNIEnv *env, jobject obj, jint baudrate) {
    speed_t sysBaudrate;
    switch (baudrate) {
        case 1200:
            sysBaudrate = B1200;
            break;
        case 2400:
            sysBaudrate = B2400;
            break;
        case 4800:
            sysBaudrate = B4800;
            break;
        case 9600:
            sysBaudrate = B9600;
            break;
        case 19200:
            sysBaudrate = B19200;
            break;
        case 38400:
            sysBaudrate = B38400;
            break;
        case 57600:
            sysBaudrate = B57600;
            break;
        case 115200:
            sysBaudrate = B115200;
            break;
        default:
            __android_log_print(ANDROID_LOG_ERROR, UART_TAG, "Invalid Uart baudrate");
            return false;
    }

    jint fileDescriptor = getIntFieldNative(env, obj, PACKAGE_PATH"/Uart", "fileDescriptor");

    /* Get and modify current option */
    struct termios option;
    fcntl(fileDescriptor, F_SETFL, O_RDWR);
    if (0 > tcgetattr(fileDescriptor, &option)) {
        __android_log_print(ANDROID_LOG_ERROR, UART_TAG, "Failed to set baudrate of %s to %d",
                            getUartNameNative(env, obj), baudrate);
        return false;
    }

    cfmakeraw(&option);
    cfsetspeed(&option, sysBaudrate);

    if (0 > tcsetattr(fileDescriptor, TCSANOW, &option)) {
        __android_log_print(ANDROID_LOG_ERROR, UART_TAG, "Failed to set baudrate of %s to %d",
                            getUartNameNative(env, obj), baudrate);
        return false;
    }

    return true;
}

JNIEXPORT jint JNICALL
JNICREATE(Uart, isDataAvailable)
        (JNIEnv *env, jobject obj) {
    jint fileDescriptor = getIntFieldNative(env, obj, PACKAGE_PATH"/Uart", "fileDescriptor");
    jint byteNumber;
    if (0 > ioctl(fileDescriptor, FIONREAD, &byteNumber)) {
        __android_log_print(ANDROID_LOG_ERROR, UART_TAG,
                            "Failed to get the number of available data bytes in RX buffer of %s",
                            getUartNameNative(env, obj));
        return -1;
    } else return byteNumber;
}

JNIEXPORT jbyteArray JNICALL
JNICREATE(Uart, read)
        (JNIEnv *env, jobject obj, jint length) {
    jint available = JNICREATE(Uart, isDataAvailable)(env, obj);
    if (length > available) length = available;

    if (0 == length) {
        /* Prevent crash Android App */
        jbyte buffer[1] = "\0";
        jbyteArray data = (*env)->NewByteArray(env, 1);
        (*env)->SetByteArrayRegion(env, data, 0, 1, buffer);
        return data;
    }

    jint fileDescriptor = getIntFieldNative(env, obj, PACKAGE_PATH"/Uart", "fileDescriptor");
    jbyte buffer[length];

    if (0 > read(fileDescriptor, buffer, length)) {
        __android_log_print(ANDROID_LOG_ERROR, UART_TAG, "Failed to read data from %s",
                            getUartNameNative(env, obj));
    }

    jbyteArray data = (*env)->NewByteArray(env, length);
    (*env)->SetByteArrayRegion(env, data, 0, length, buffer);
    return data;
}

JNIEXPORT jboolean JNICALL
JNICREATE(Uart, write)
        (JNIEnv *env, jobject obj, jbyteArray data) {
    jint fileDescriptor = getIntFieldNative(env, obj, PACKAGE_PATH"/Uart", "fileDescriptor");
    jbyte *byteData = (*env)->GetByteArrayElements(env, data, 0);
    jsize length = (*env)->GetArrayLength(env, data);

    if (0 > write(fileDescriptor, byteData, length)) {
        __android_log_print(ANDROID_LOG_ERROR, UART_TAG, "Failed to write data to %s",
                            getUartNameNative(env, obj));
        return false;
    } else return true;
}

JNIEXPORT jboolean JNICALL
JNICREATE(Uart, flush)
        (JNIEnv *env, jobject obj, jint type) {
    jint fileDescriptor = getIntFieldNative(env, obj, PACKAGE_PATH"/Uart", "fileDescriptor");
    if (0 > tcflush(fileDescriptor, type)) {
        __android_log_print(ANDROID_LOG_ERROR, UART_TAG, "Failed to flush buffer of %s",
                            getUartNameNative(env, obj));
        return false;
    } else return true;
}
