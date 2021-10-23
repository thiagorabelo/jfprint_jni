/*
 * defs.cpp
 *
 *  Created on: 03 de set. de 2021
 *      Author: thiago
 */


#include <jni_utils.h>
#include <jfprint_FpPrint.h>
#include <fprint_abstraction/Print.h>

#include <functional>
#include <string>
#include <utility>


JNIEXPORT void JNICALL Java_jfprint_FpPrint_nativeClose
  (JNIEnv *env, jobject self)
{
    Print *dev = jni::pointer::getPointerAddress<Print>(env, self, "pointer");
    delete dev;
}


JNIEXPORT jint JNICALL Java_jfprint_FpPrint_native_1get_1finger
  (JNIEnv *env, jobject self)
{
    Print *dev = jni::pointer::getPointerAddress<Print>(env, self, "pointer");
    FpFinger finger = dev->get_finger();
    return static_cast<int>(finger);
}
