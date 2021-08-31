/*
 * defs.cpp
 *
 *  Created on: 30 de ago. de 2021
 *      Author: thiago
 */

#include <jni.h>


#define CLASS_BYTE_BUFFER "Ljava/nio/ByteBuffer;"


#define CLASS_SIGN(cls) "Ljfprint/" cls ";"

#define CLASS_FPCONTEXT  CLASS_SIGN("FpContext")
#define CLASS_FPDEVICE   CLASS_SIGN("FpDevice")


namespace jni
{
	jobject newInstance(JNIEnv *env, const char *clsName) noexcept;
	jobject newInstance(JNIEnv *env, jclass cls) noexcept;

	namespace pointer
	{
        template<typename T>
		T* setPointerAddress(JNIEnv *env, jobject obj, const char *fieldName, T *address) noexcept
		{
			jclass cls = env->GetObjectClass(obj);
			jfieldID fieldId = env->GetFieldID(cls, fieldName, CLASS_BYTE_BUFFER);
			jobject oldPtr = env->GetObjectField(obj, fieldId);
			jobject buffer = env->NewDirectByteBuffer(address, sizeof(T*));
			env->SetObjectField(obj, fieldId, buffer);

			if (!oldPtr) {
				return nullptr;
			}

			return reinterpret_cast<T*>(env->GetDirectBufferAddress(oldPtr));
		}

        template <typename T>
        T* getPointerAddress(JNIEnv *env, jobject obj, const char *fieldName) noexcept
		{
			jclass cls = env->GetObjectClass(obj);
			jfieldID fieldId = env->GetFieldID(cls, fieldName, CLASS_BYTE_BUFFER);
			jobject buffer = env->GetObjectField(obj, fieldId);
			return reinterpret_cast<T*>(env->GetDirectBufferAddress(buffer));
		}
	};
}