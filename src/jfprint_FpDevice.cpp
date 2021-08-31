/*
 * defs.cpp
 *
 *  Created on: 30 de ago. de 2021
 *      Author: thiago
 */


#include <jni_utils.h>
#include <jfprint_FpDevice.h>
#include <fprint_abstraction/Device.h>

#include <utility>
#include <string>
#include <functional>


jstring get_jstring_value(JNIEnv *env, jobject obj, std::function<std::string(Device *dev)> get_string)
{
    Device *dev = jni::pointer::getPointerAddress<Device>(env, obj, "pointer");
    std::string str = get_string(dev);
    return env->NewStringUTF(str.c_str());
}


JNIEXPORT void JNICALL Java_jfprint_FpDevice_nativeClose
  (JNIEnv *env, jobject self)
{
    Device *dev = jni::pointer::getPointerAddress<Device>(env, self, "pointer");
    delete dev;
}


JNIEXPORT jstring JNICALL Java_jfprint_FpDevice_get_1driver
  (JNIEnv *env, jobject self)
{
    return get_jstring_value(env, self, [](Device *dev) {
        return dev->get_driver();
    });
}


JNIEXPORT jstring JNICALL Java_jfprint_FpDevice_get_1device_1id
  (JNIEnv *env, jobject self)
{
    return get_jstring_value(env, self, [](Device *dev) {
        return dev->get_device_id();
    });
}


JNIEXPORT jstring JNICALL Java_jfprint_FpDevice_get_1name
  (JNIEnv *env, jobject self)
{
    return get_jstring_value(env, self, [](Device *dev){
        return dev->get_name();
    });
}
