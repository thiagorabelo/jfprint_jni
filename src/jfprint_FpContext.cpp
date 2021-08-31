/*
 * defs.cpp
 *
 *  Created on: 30 de ago. de 2021
 *      Author: thiago
 */


#include <jfprint_FpContext.h>
#include <fprint_abstraction/Context.h>
#include <fprint_abstraction/Device.h>
#include <jni_utils.h>

#include <utility>


JNIEXPORT void JNICALL Java_jfprint_FpContext_nativeClose
  (JNIEnv *env, jobject self)
{
    Context *ctx = jni::pointer::getPointerAddress<Context>(env, self, "pointer");
    delete ctx;
}

JNIEXPORT jobject JNICALL Java_jfprint_FpContext_context_1new
  (JNIEnv *env, jclass cls)
{
    Context *ctx = new Context(std::move(Context::context_new()));
    jobject obj = jni::newInstance(env, cls);
    jni::pointer::setPointerAddress<Context>(env, obj, "pointer", ctx);
    return obj;
}


JNIEXPORT jint JNICALL Java_jfprint_FpContext_n_1devices
  (JNIEnv *env, jobject self)
{
    Context *ctx = jni::pointer::getPointerAddress<Context>(env, self, "pointer");
    return static_cast<int>(ctx->n_devices());
}

JNIEXPORT jobject JNICALL Java_jfprint_FpContext_get_1device
  (JNIEnv *env, jobject self, jint index)
{
	Context *ctx = jni::pointer::getPointerAddress<Context>(env, self, "pointer");
    Device *device = new Device(std::move(ctx->get_device(static_cast<guint>(index))));
    jobject obj = jni::newInstance(env, CLASS_FPDEVICE);
    jni::pointer::setPointerAddress<Device>(env, obj, "pointer", device);
    return obj;
}
