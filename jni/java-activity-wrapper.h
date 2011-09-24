#include <jni.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _Included_com_sonyericsson_zsystem_jni_ZJavaActivity
#define _Included_com_sonyericsson_zsystem_jni_ZJavaActivity
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_sonyericsson_zsystem_jni_ZJavaActivity
 * Method:    JNI_InitializeActivity
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Landroid/content/res/AssetManager;)I
 */
JNIEXPORT jint JNICALL Java_com_sonyericsson_zsystem_jni_ZJavaActivity_JNI_1InitializeActivity
  (JNIEnv *, jobject, jstring, jstring, jstring, jobject);

/*
 * Class:     com_sonyericsson_zsystem_jni_ZJavaActivity
 * Method:    verifyLicense
 * Signature: (ILjava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_sonyericsson_zsystem_jni_ZJavaActivity_verifyLicense
  (JNIEnv *, jclass, jint, jstring, jstring, jlong, jstring, jstring, jstring, jint);

/*
 * Class:     com_sony_android_psone_CommonFunctions
 * Method:    a
 * Signature: (ILjava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;I)I
*/
JNIEXPORT jint JNICALL Java_com_sony_android_psone_CommonFunctions_a
  (JNIEnv *, jclass, jint, jstring, jstring, jlong, jstring, jstring, jstring, jint);

jstring getLibraryPath(JNIEnv *, jstring);

int setTocFile(JNIEnv *env, jstring sdDataDir);

void overwriteTocFunctions(const char *);

int PsCrypt_GetImageToc(void* dest);

int PsCrypt_GetImageTocLength();

#ifdef __cplusplus
}
#endif
#endif