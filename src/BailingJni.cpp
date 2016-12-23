#include "../include/BailingJni.h"
#include<iostream>

using namespace std;
using namespace cv;
// jstring To char*  
char* jstringTostring(JNIEnv* env, jstring jstr)
{
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("GB2312");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char*)malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}

// char* To jstring  
jstring stringTojstring(JNIEnv* env, const char* pat)
{
	jclass strClass = env->FindClass("Ljava/lang/String;");
		jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
		jbyteArray bytes = env->NewByteArray(strlen(pat));
		env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
		//此处用GB2312是因为char*中文这个方式保存，如果换成UTF-8认为char*是UTF-8显然不对,linux也测试成功
		jstring encoding = env->NewStringUTF("GB2312");
		return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

jstring stoJstring(JNIEnv* env, const char* pat)
{
	jclass strClass = env->FindClass("Ljava/lang/String;");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(pat));
	env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
	jstring encoding = env->NewStringUTF("utf-8");
	return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}


/*
 * Class:     org_img_processing_BailingJni
 * Method:    processing
 * Signature: (I[B)Ljava/lang/String;
 */
	JNIEXPORT jstring JNICALL Java_org_img_processing_BailingJni_processing
(JNIEnv *env, jobject object, MSERProcess *mserProcess,  jbyteArray img)
{


	jboolean isCopy = JNI_FALSE;
	int size = env->GetArrayLength(img);
	jbyte* imagebuffer = env->GetByteArrayElements(img, &isCopy);
	if(imagebuffer == NULL)
	{
		return NULL;
	}
	Mat src = imdecode(Mat(1, size, CV_8U, imagebuffer), IMREAD_COLOR);
	//MSERProcess mserProcess("tmp");

	//printf("pr=%d\n", static_cast<const void*>(mserProcess));
	//printf("process ptr=%p\n", &mserProcess);
	char* result = mserProcess->doGetTxt(src);

	jstring jresult = stoJstring(env, result);
	return jresult;

}

/*
 * Class:     org_img_processing_BailingJni
 * Method:    initPath
 * Signature: (Ljava/lang/String;)I
 */
	JNIEXPORT MSERProcess* JNICALL Java_org_img_processing_BailingJni_initPath
(JNIEnv *env, jobject object, jstring path)
{
	string modelpath = jstringTostring(env, path);
	char pathTmp[65535];
	strcpy(pathTmp, modelpath.c_str());

	MSERProcess *ptr = new MSERProcess(pathTmp);
//	printf("init ptr=%p\n", &ptr);
	//Mat img = imread("/root/1xiu/bailingOCR2.0/imgs/yy.jpg", 1);
	//char* result = ptr->doGetTxt(img);
	//printf("res=%s\n", result);

	return ptr;
}
JNIEXPORT void JNICALL Java_org_img_processing_BailingJni_delete
(JNIEnv *env, jobject object, MSERProcess * ptr)
{
	delete ptr;
}
