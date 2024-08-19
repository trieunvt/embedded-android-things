# Embedded Android Things

Embedded Android Things provides Java-level APIs for accessing generic device peripherals on Android-based embedded operating systems. Inspired by Google Android Things and OpenJDK Device I/O Project, Embedded Android Things is designed based on Java Native Interface (JNI) and implemented on Android Open Source Project (AOSP).

## How To Use

-   Copy these folders `cpp` and `java` to your application main folder.
-   Change the hierarchy structure of the folder `java` based on your application package.
-   Execute the shell script `java/../../../EmbeddedAndroidThings/GenerateNativeHeaderFiles.sh` inside your application main folder.
-   Change these macros `JNIDEFINE`, `JNICREATE` and `PACKAGE_PATH` in the native header file `cpp/EmbeddedAndroidThings/Include/Essentials.h` based on your application package.
-   Build your application and enjoy.

## Sample Application

-   [Android Sample Application](./Application/app/src/main/)
