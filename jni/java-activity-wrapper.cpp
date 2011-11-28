/*
 * PSXperia Emulator Wrapper
 * Copyright (C) 2011 Yifan Lu (http://yifan.lu/)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <java-activity-wrapper.h>
//#include <android/log.h>

#define DATA_BASE "/data/data/"
#define TOC_FILE "/files/content/image_ps_toc.bin"
#define ORIGINAL_LIB "/lib/libjava-activity-patched.so"

FILE *tocFile;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved)
{
	return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *jvm, void *reserved)
{
	if(tocFile)
	{
		fclose(tocFile);
	}
}

JNIEXPORT jint JNICALL Java_com_sonyericsson_zsystem_jni_ZJavaActivity_JNI_1InitializeActivity
  (JNIEnv *env, jobject obj, jstring dataDir, jstring sdDataDir, jstring packageName, jobject assistMgr)
{	
	return JNI_InitializeActivity("com/sonyericsson/zsystem/jni/ZJavaActivity", "JNI_InitializeActivity", env, obj, dataDir, sdDataDir, packageName, assistMgr);
}

JNIEXPORT jint JNICALL Java_com_sce_zsystem_jni_ZPlatformActivity_JNI_1InitializeActivity
  (JNIEnv *env, jobject obj, jstring dataDir, jstring sdDataDir, jstring packageName, jobject assistMgr)
{
	return JNI_InitializeActivity("com/sce/zsystem/jni/ZPlatformActivity", "JNI_InitializeActivity", env, obj, dataDir, sdDataDir, packageName, assistMgr);
}

JNIEXPORT jint JNICALL JNI_InitializeActivity
  (const char *className, const char *methodName, JNIEnv *env, jobject obj, jstring dataDir, jstring sdDataDir, jstring packageName, jobject assistMgr)
{	
	jclass cls;
	jmethodID get_load_id;
	jstring libPath;
	
	cls = env->FindClass("java/lang/System");
	get_load_id = env->GetStaticMethodID(cls, "load", "(Ljava/lang/String;)V");
	libPath = getLibraryPath(env, packageName);
	
	/*
	const char *nativeString = env->GetStringUTFChars(libPath, NULL);
	__android_log_print(ANDROID_LOG_DEBUG, "PSEmulatorWrapper", "Lib path, %s", nativeString);
	env->ReleaseStringUTFChars(libPath, nativeString);
	*/
	
	//__android_log_print(ANDROID_LOG_DEBUG, "PSEmulatorWrapper", "%s", "We are init-ing the emulator.");
	
	setTocFile(env, sdDataDir);
	
	env->CallStaticVoidMethod(cls, get_load_id, libPath);
	
	cls = env->FindClass(className);
	get_load_id = env->GetMethodID(cls, methodName, "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Landroid/content/res/AssetManager;)I");
	
	//__android_log_print(ANDROID_LOG_DEBUG, "PSEmulatorWrapper", "Wrapper loaded successfully, %p", env);
	return env->CallIntMethod(obj, get_load_id, dataDir, sdDataDir, packageName, assistMgr);
}

JNIEXPORT jint JNICALL Java_com_sony_android_psone_CommonFunctions_a // verifyLicense
  (JNIEnv *env, jclass cls1, jint int1, jstring deviceStr, jstring signature, jlong long1, jstring packageName, jstring str4, jstring str5, jint int2)
{
	return verifyLicense("a", env, cls1, int1, deviceStr, signature, long1, packageName, str4, str5, int2);
}

JNIEXPORT jint JNICALL Java_com_sonyericsson_zsystem_jni_ZJavaActivity_verifyLicense
  (JNIEnv *env, jclass cls1, jint int1, jstring deviceStr, jstring signature, jlong long1, jstring packageName, jstring str4, jstring str5, jint int2)
{
	return verifyLicense("verifyLicense", env, cls1, int1, deviceStr, signature, long1, packageName, str4, str5, int2);
}

JNIEXPORT jint JNICALL verifyLicense
  (const char *methodName, JNIEnv *env, jclass cls1, jint int1, jstring deviceStr, jstring signature, jlong long1, jstring packageName, jstring str4, jstring str5, jint int2)
{
	jclass cls;
	jmethodID get_load_id;
	jstring dataDirBase;
	jstring libPath;
	
	cls = env->FindClass("java/lang/System");
	get_load_id = env->GetStaticMethodID(cls, "load", "(Ljava/lang/String;)V");
	libPath = getLibraryPath(env, packageName);
	
	/*
	const char *nativeString = env->GetStringUTFChars(libPath, NULL);
	__android_log_print(ANDROID_LOG_DEBUG, "PSEmulatorWrapper", "Lib path, %s", nativeString);
	env->ReleaseStringUTFChars(libPath, nativeString);
	*/
	
	//__android_log_print(ANDROID_LOG_DEBUG, "PSEmulatorWrapper", "%s", "We are verifying the license.");
	
	env->CallStaticVoidMethod(cls, get_load_id, libPath);
	
	get_load_id = env->GetStaticMethodID(cls1, methodName, "(ILjava/lang/String;Ljava/lang/String;JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;I)I");
	
	return env->CallStaticIntMethod(cls1, get_load_id, int1, deviceStr, signature, long1, packageName, str4, str5, int2);
}

jstring getLibraryPath(JNIEnv *env, jstring packageName)
{
	jclass stringClass;
	jmethodID cid;
	jstring dataDirBase;
	jobject libPath;
	jstring libName;
	
	dataDirBase = env->NewStringUTF(DATA_BASE);
	libName = env->NewStringUTF(ORIGINAL_LIB);
	stringClass = env->FindClass("java/lang/String");
	if (stringClass == NULL)
		return NULL;
	cid = env->GetMethodID(stringClass, "<init>", "(Ljava/lang/String;)V");
	if (cid == NULL)
		return NULL;
	libPath = env->NewObject(stringClass, cid, dataDirBase);
	cid = env->GetMethodID(stringClass, "concat", "(Ljava/lang/String;)Ljava/lang/String;");
	if (cid == NULL)
		return NULL;
	libPath = env->CallObjectMethod(libPath, cid, packageName);
	libPath = env->CallObjectMethod(libPath, cid, libName);

	return (jstring)libPath;
}

int setTocFile(JNIEnv *env, jstring sdDataDir)
{
	const char *cSdDataPath = env->GetStringUTFChars(sdDataDir, NULL);
	if(!cSdDataPath)
		return 1;
	const char *tocLoc = TOC_FILE;
	int len = strlen(cSdDataPath) + strlen(tocLoc);
	char path[len];
	strcpy(path, cSdDataPath);
	strcat(path, tocLoc);
	tocFile = fopen(path, "rb");
	env->ReleaseStringUTFChars(sdDataDir, cSdDataPath);
	if(!tocFile)
		return 1;
	return 0;
}

int PsCrypt_GetImageToc(void* dest)
{
	if(!tocFile)
		return -1;
	return fread(dest, sizeof(char), PsCrypt_GetImageTocLength(), tocFile);
}

int PsCrypt_GetImageTocLength()
{
	if(!tocFile)
		return -1;
	int filesize;
	fseek(tocFile, 0L, SEEK_END);
	filesize = ftell(tocFile);
	fseek(tocFile, 0L, SEEK_SET);
	return filesize;
}
