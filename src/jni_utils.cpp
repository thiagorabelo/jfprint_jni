/*
 * defs.cpp
 *
 *  Created on: 30 de ago. de 2021
 *      Author: thiago
 */

#include <jni_utils.h>


namespace jni
{
	jobject newInstance(JNIEnv *env, const char *clsName) noexcept
	{
		jclass cls = env->FindClass(clsName);
		return jni::newInstance(env, cls);
	}

	jobject newInstance(JNIEnv *env, jclass cls) noexcept
	{
		jmethodID initID = env->GetMethodID(cls, "<init>", "()V");
		jobject instance = env->NewObject(cls, initID);
		return instance;
	}

}
