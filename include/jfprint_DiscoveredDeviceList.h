/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class jfprint_DiscoveredDeviceList */

#ifndef _Included_jfprint_DiscoveredDeviceList
#define _Included_jfprint_DiscoveredDeviceList
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     jfprint_DiscoveredDeviceList
 * Method:    nativeClose
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_jfprint_DiscoveredDeviceList_nativeClose
  (JNIEnv *, jobject);

/*
 * Class:     jfprint_DiscoveredDeviceList
 * Method:    fp_get
 * Signature: (I)Ljfprint/DiscoveredDevice;
 */
JNIEXPORT jobject JNICALL Java_jfprint_DiscoveredDeviceList_fp_1get
  (JNIEnv *, jobject, jint);

/*
 * Class:     jfprint_DiscoveredDeviceList
 * Method:    fp_dicoverDevices
 * Signature: ()Ljfprint/DiscoveredDeviceList;
 */
JNIEXPORT jobject JNICALL Java_jfprint_DiscoveredDeviceList_fp_1dicoverDevices
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
