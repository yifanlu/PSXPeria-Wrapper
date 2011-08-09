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

#define TOC_FILE "/files/content/image_ps_toc.bin"
#define ORIGINAL_LIB "/lib/libjava-activity.so"

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
	jclass cls;
	jmethodID get_load_id;
	jstring libPath;
	const char *cLibPath;
	
	cls = env->FindClass("java/lang/System");
	get_load_id = env->GetStaticMethodID(cls, "load", "(Ljava/lang/String;)V");
	libPath = getLibraryPath(env, dataDir);
	
	setTocFile(env, sdDataDir);
	
	env->CallStaticVoidMethod(cls, get_load_id, libPath);
	
	cLibPath = env->GetStringUTFChars(libPath, NULL);
	env->ReleaseStringUTFChars(libPath, cLibPath);
	
	cls = env->FindClass("com/sonyericsson/zsystem/jni/ZJavaActivity");
	get_load_id = env->GetMethodID(cls, "JNI_InitializeActivity", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Landroid/content/res/AssetManager;)I");
	
	//__android_log_print(ANDROID_LOG_DEBUG, "PSEmulatorWrapper", "Wrapper loaded successfully, %p", env);
	return env->CallIntMethod(obj, get_load_id, dataDir, sdDataDir, packageName, assistMgr);
}

jstring getLibraryPath(JNIEnv *env, jstring dataDir)
{
	jclass stringClass;
	jmethodID cid;
	jobject libPath;
	jstring libName;

	libName = env->NewStringUTF(ORIGINAL_LIB);

	stringClass = env->FindClass("java/lang/String");
	if (stringClass == NULL)
		return NULL;
	cid = env->GetMethodID(stringClass, "<init>", "(Ljava/lang/String;)V");
	if (cid == NULL)
		return NULL;
	libPath = env->NewObject(stringClass, cid, dataDir);
	cid = env->GetMethodID(stringClass, "concat", "(Ljava/lang/String;)Ljava/lang/String;");
	if (cid == NULL)
		return NULL;
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
	if(!tocFile)
		return 1;
	return 0;
}

/*
INCOMPLETE, NOT NEEDED ANYMORE
void overwriteTocFunctions(const char *libPath)
{	
	int (*PsCrypt_GetImageToc)(void*) = NULL;
	int (*PsCrypt_GetImageTocLength)() = NULL;
	void *handle;
	void *tocPtr;
	int length;
	
	//handle = dlopen(libPath, RTLD_LAZY);
	*(void **)(&PsCrypt_GetImageToc) = dlsym((void*)0xFFFFFFFF, "PsCrypt_GetImageToc");
	*(void **)(&PsCrypt_GetImageTocLength) = dlsym((void*)0xFFFFFFFF, "PsCrypt_GetImageTocLength");
}
*/

int PsCrypt_GetImageToc(void* dest)
{
	if(!tocFile)
		return 0;
	return fread(dest, sizeof(char), PsCrypt_GetImageTocLength(), tocFile);
}

int PsCrypt_GetImageTocLength()
{
	if(!tocFile)
		return 0;
	int filesize;
	fseek(tocFile, 0L, SEEK_END);
	filesize = ftell(tocFile);
	fseek(tocFile, 0L, SEEK_SET);
	return filesize;
}