/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_example_application_EmbeddedAndroidThings_PeripheralManager */

#ifndef _Included_com_example_application_EmbeddedAndroidThings_PeripheralManager
#define _Included_com_example_application_EmbeddedAndroidThings_PeripheralManager
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_example_application_EmbeddedAndroidThings_PeripheralManager
 * Method:    getGpioListNative
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_com_example_application_EmbeddedAndroidThings_PeripheralManager_getGpioListNative
  (JNIEnv *, jobject);

/*
 * Class:     com_example_application_EmbeddedAndroidThings_PeripheralManager
 * Method:    getI2cListNative
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_com_example_application_EmbeddedAndroidThings_PeripheralManager_getI2cListNative
  (JNIEnv *, jobject);

/*
 * Class:     com_example_application_EmbeddedAndroidThings_PeripheralManager
 * Method:    getUartListNative
 * Signature: ()[I
 */
JNIEXPORT jintArray JNICALL Java_com_example_application_EmbeddedAndroidThings_PeripheralManager_getUartListNative
  (JNIEnv *, jobject);

/*
 * Class:     com_example_application_EmbeddedAndroidThings_PeripheralManager
 * Method:    openGpioNative
 * Signature: (Lcom/example/application/EmbeddedAndroidThings/Gpio;II)Lcom/example/application/EmbeddedAndroidThings/Gpio;
 */
JNIEXPORT jobject JNICALL Java_com_example_application_EmbeddedAndroidThings_PeripheralManager_openGpioNative
  (JNIEnv *, jobject, jobject, jint, jint);

/*
 * Class:     com_example_application_EmbeddedAndroidThings_PeripheralManager
 * Method:    openI2cNative
 * Signature: (Lcom/example/application/EmbeddedAndroidThings/I2c;II)Lcom/example/application/EmbeddedAndroidThings/I2c;
 */
JNIEXPORT jobject JNICALL Java_com_example_application_EmbeddedAndroidThings_PeripheralManager_openI2cNative
  (JNIEnv *, jobject, jobject, jint, jint);

/*
 * Class:     com_example_application_EmbeddedAndroidThings_PeripheralManager
 * Method:    openUartNative
 * Signature: (Lcom/example/application/EmbeddedAndroidThings/Uart;I)Lcom/example/application/EmbeddedAndroidThings/Uart;
 */
JNIEXPORT jobject JNICALL Java_com_example_application_EmbeddedAndroidThings_PeripheralManager_openUartNative
  (JNIEnv *, jobject, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
