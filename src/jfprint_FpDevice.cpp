/*
 * defs.cpp
 *
 *  Created on: 30 de ago. de 2021
 *      Author: thiago
 */


#include <jni_utils.h>
#include <jfprint_FpDevice.h>
#include <fprint_abstraction/Device.h>
#include <fprint_abstraction/Print.h>
#include <fprint_abstraction/PrintTemplate.h>

#include <functional>
#include <string>
#include <utility>


jstring get_jstring_value(JNIEnv *env, jobject obj, std::function<std::string(Device *dev)> get_string)
{
    Device *dev = jni::pointer::getPointerAddress<Device>(env, obj);
    std::string str = get_string(dev);
    return env->NewStringUTF(str.c_str());
}


JNIEXPORT void JNICALL Java_jfprint_FpDevice_nativeClose
  (JNIEnv *env, jobject self)
{
    Device *dev = jni::pointer::getPointerAddress<Device>(env, self);
    delete dev;
}


JNIEXPORT jstring JNICALL Java_jfprint_FpDevice_native_1get_1driver
  (JNIEnv *env, jobject self)
{
    return get_jstring_value(env, self, [](Device *dev) {
        return dev->get_driver();
    });
}


JNIEXPORT jstring JNICALL Java_jfprint_FpDevice_native_1get_1device_1id
  (JNIEnv *env, jobject self)
{
    return get_jstring_value(env, self, [](Device *dev) {
        return dev->get_device_id();
    });
}


JNIEXPORT jstring JNICALL Java_jfprint_FpDevice_native_1get_1name
  (JNIEnv *env, jobject self)
{
    return get_jstring_value(env, self, [](Device *dev){
        return dev->get_name();
    });
}


JNIEXPORT jboolean JNICALL Java_jfprint_FpDevice_native_1open
  (JNIEnv *env, jobject self)
{
    Device *dev = jni::pointer::getPointerAddress<Device>(env, self);
    return dev->open();
}


JNIEXPORT jboolean JNICALL Java_jfprint_FpDevice_native_1close
  (JNIEnv *env, jobject self)
{
    Device *dev = jni::pointer::getPointerAddress<Device>(env, self);
    return dev->close();
}

JNIEXPORT jobject JNICALL Java_jfprint_FpDevice_native_1enroll
  (JNIEnv *env, jobject self, jobject progress, jobject jprint_template)
{
    Device *dev = jni::pointer::getPointerAddress<Device>(env, self);

    auto lambda = [env, progress, self](FpDevice *device,
                                        gint completed_stages,
                                        FpPrint *print,
                                        gpointer user_data,
                                        GError *error)
    {
    	debug("Pointers: device=", device, ", user_data=", user_data, " e error=", error);

        if (progress) {
            jmethodID methodId = jni::jfprint::getProgressCallbackMethod(env, progress);
            jobject jprint = jni::newInstance(env, CLASS_FPPRINT);

            // TODO: What happens if garbage collector was called and FpPrint *print is already cleaned?
            jni::pointer::setPointerAddress<Print>(env, jprint, new Print(print));
            env->CallVoidMethod(progress, methodId, self, completed_stages, jprint);
        }
    };
    PrintTemplate print_template = jni::jfprint::getPrintTemplateFromJavaPrintTemplate(env, jprint_template);
    Print *print = new Print(std::move(dev->enroll(lambda, print_template)));

    // utils::simple_date s = print->get_enroll_date();
    // debug("Print Data: finger=", print->get_finger(),
    //       ", username=\"", print->get_username(), "\""
    //       ", data=\"", s.year, "-", s.month, "-", s.day, "\"");

    jobject obj = jni::newInstance(env, CLASS_FPPRINT);
    jni::pointer::setPointerAddress<Print>(env, obj, print);
    return obj;
}
