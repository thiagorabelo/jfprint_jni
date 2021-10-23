/*
 * defs.cpp
 *
 *  Created on: 30 de ago. de 2021
 *      Author: thiago
 */

#include <jni.h>
#include <fprint_abstraction/PrintTemplate.h>
#include <fprint_abstraction/utils.h>

#define CLASS_BYTE_BUFFER "Ljava/nio/ByteBuffer;"


#define CLASS_SIGN(cls) "Ljfprint/" cls ";"

#define CLASS_FPCONTEXT  CLASS_SIGN("FpContext")
#define CLASS_FPDEVICE   CLASS_SIGN("FpDevice")
#define CLASS_FPPRINT    CLASS_SIGN("FpPrint")

#define ENEROLL_PROGRESS_INTERFACE_METHOD_SIGNATURE "(Ljfprint/FpDevice;ILjfprint/FpPrint;)V"


namespace jni
{
	jobject newInstance(JNIEnv *env, const char *clsName) noexcept;
	jobject newInstance(JNIEnv *env, jclass cls) noexcept;

    jmethodID getMethod(JNIEnv *env, jobject object, const char* name, const char* signature) noexcept;


    class jCalendarFieldGetter
    {
        public:
            jCalendarFieldGetter(JNIEnv *env, jobject jcalendar);

            jint get(const char *fieldName);

        private:
            JNIEnv *env;
            jobject jcalendar;
            jclass cls;
            jmethodID get_id;
    };


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

		template<typename T>
		T* setPointerAddress(JNIEnv *env, jobject obj, T *address) noexcept
		{
			return jni::pointer::setPointerAddress(env, obj, "pointer", address);
		}

        template<typename T>
        T* getPointerAddress(JNIEnv *env, jobject obj) noexcept
        {
        	return jni::pointer::getPointerAddress<T>(env, obj, "pointer");
        }
	};


    namespace jfprint
    {
        utils::simple_date getSimpleDateFromJavaCalendar(jobject jdate);

        jmethodID getProgressCallbackMethod(JNIEnv *env, jobject progress);

        PrintTemplate getPrintTemplateFromJavaPrintTemplate(JNIEnv *env, jobject print_template);
    };
}
