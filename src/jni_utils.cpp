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

    jmethodID getMethod(JNIEnv *env, jobject object, const char* name, const char* signature) noexcept
    {
        jclass cls = env->GetObjectClass(object);
        jmethodID methodId = env->GetMethodID(cls, name, signature);
        return methodId;
    }

    jCalendarFieldGetter::jCalendarFieldGetter(JNIEnv *env, jobject jcalendar)
        : env(env), jcalendar(jcalendar)
    {
        cls = env->GetObjectClass(jcalendar);
        get_id = env->GetMethodID(cls, "get", "(I)I");
    }

    jint jCalendarFieldGetter::get(const char *fieldName)
    {
        jfieldID field_id = env->GetStaticFieldID(cls, fieldName, "I");
        jint static_field_value = env->GetStaticIntField(cls, field_id);
        jint value = env->CallIntMethod(jcalendar, get_id, static_field_value);
        return value;
    }


    namespace jfprint
    {
        utils::simple_date getSimpleDateFromJavaCalendar(JNIEnv *env, jobject jcalendar)
        {
            jni::jCalendarFieldGetter getter(env, jcalendar);

            jint year = getter.get("YEAR");
            jint month = getter.get("MONTH");
            jint day = getter.get("DAY_OF_MONTH");

            return {
                static_cast<int>(year),
                static_cast<int>(month) + 1, // Adjust: 0-11 to 1-12
                static_cast<int>(day)
            };
        }

        jmethodID getProgressCallbackMethod(JNIEnv *env, jobject progress)
        {
            return jni::getMethod(env, progress, "callback", ENEROLL_PROGRESS_INTERFACE_METHOD_SIGNATURE);
        }

        PrintTemplate getPrintTemplateFromJavaPrintTemplate(JNIEnv *env, jobject print_template)
        {
            PrintTemplate pt;

            jclass cls = env->GetObjectClass(print_template);

            // set_finger
            jmethodID get_finger_id = env->GetMethodID(cls, "getFinger", "()I");
            jint finger = env->CallIntMethod(print_template, get_finger_id);
            pt.set_finger(static_cast<FpFinger>(finger));

            // set_username
            jmethodID get_username_id = env->GetMethodID(cls, "getUsername", "()Ljava/lang/String;");
            jstring jusername = reinterpret_cast<jstring>(env->CallObjectMethod(print_template, get_username_id));
            
            if (jusername) {
                const char* username = env->GetStringUTFChars(jusername, nullptr);
                pt.set_username(username);
                env->ReleaseStringUTFChars(jusername, username);
            }

            // set_description
            jmethodID get_description_id = env->GetMethodID(cls, "getDescription", "()Ljava/lang/String;");
            jstring jdescription = reinterpret_cast<jstring>(env->CallObjectMethod(print_template, get_description_id));
            
            if (jdescription) {
                const char* description = env->GetStringUTFChars(jdescription, nullptr);
                pt.set_description(description);
                env->ReleaseStringUTFChars(jdescription, description);
            }

            // set_enroll_date
            jmethodID get_calendar_id = env->GetMethodID(cls, "getCalendar", "()Ljava/util/Calendar;");
            jobject jcalendar = env->CallObjectMethod(print_template, get_calendar_id);

            if (jcalendar) {
                utils::simple_date date = jni::jfprint::getSimpleDateFromJavaCalendar(env, jcalendar);
                pt.set_enroll_date(date.day, date.month, date.year);
            }

            return pt;
        }
    };
}
