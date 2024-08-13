#include "Include/Essentials.h"

jint getIntFieldNative (JNIEnv *env, jobject obj,
                        const char *classpath, const char *field) {
    jclass class = (*env)->FindClass(env, classpath);
    jfieldID fieldId = (*env)->GetFieldID(env, class, field, "I");
    return (*env)->GetIntField(env, obj, fieldId);
}

void setIntFieldNative (JNIEnv *env, jobject obj,
                        const char *classpath, const char *field, jint value) {
    jclass class = (*env)->FindClass(env, classpath);
    jfieldID fieldId = (*env)->GetFieldID(env, class, field, "I");
    (*env)->SetIntField(env, obj, fieldId, value);
}
