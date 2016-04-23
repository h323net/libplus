/*
* libplus_jni.cpp
*
* Copyright (c) 2016 ISVO (Asia) Pte Ltd. All Rights Reserved.
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, you can obtain one at http://mozilla.org/MPL/2.0/
*
* Software distributed under the License is distributed on an "AS IS"
* basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
* the License for the specific language governing rights and limitations
* under the License.
*
* Contributor(s): ______________________________________.
*
*
*/

#include "pch.h"
#include "libplus_jni.h"


#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "libPLUSjni", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "libPLUSjni", __VA_ARGS__))

JNI_PLUSdevice * m_plus = NULL;

//////////////////////////////////////////////////////////////////////
// TODO
// Simplify the macros 
//////////////////////////////////////////////////////////////////////

#define JNI_HEADER(name) \
extern "C" { \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_##name(JNIEnv* env, jobject thiz); \
}; \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_##name(JNIEnv* env, jobject thiz)

#define JNI_HEADER_SETVOID(name) \
extern "C" { \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_set##name(JNIEnv* env, jobject thiz, jstring value); \
}; \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_set##name(JNIEnv* env, jobject thiz, jstring value)

#define JNI_HEADER_GETJSTRING(name) \
extern "C" { \
JNIEXPORT jstring JNICALL Java_com_libPLUSandroid_libPLUSandroid_get##name(JNIEnv* env, jobject thiz); \
}; \
JNIEXPORT jstring JNICALL Java_com_libPLUSandroid_libPLUSandroid_get##name(JNIEnv* env, jobject thiz)

#define JNI_HEADER_DO(name) \
extern "C" { \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_do##name(JNIEnv* env, jobject thiz); \
}; \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_do##name(JNIEnv* env, jobject thiz)

#define JNI_HEADER_DO1(name) \
extern "C" { \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_do##name(JNIEnv* env, jobject thiz, jstring p1); \
}; \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_do##name(JNIEnv* env, jobject thiz, jstring p1)

#define JNI_HEADER_DO2(name) \
extern "C" { \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_do##name(JNIEnv* env, jobject thiz, jstring p1, jstring p2); \
}; \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_do##name(JNIEnv* env, jobject thiz, jstring p1, jstring p2)

#define JNI_HEADER_DO3(name) \
extern "C" { \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_do##name(JNIEnv* env, jobject thiz, jstring p1, jstring p2, jstring p3); \
}; \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_do##name(JNIEnv* env, jobject thiz, jstring p1, jstring p2, jstring p3)

#define JNI_HEADER_MEDIA(name) \
extern "C" { \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_do##name(JNIEnv *env, jobject obj, jbyteArray data, jint size, jint width, jint height); \
}; \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_do##name(JNIEnv *env, jobject obj, jbyteArray data, jint size, jint width, jint height)



#define JNI_PLUS_CHECK_VOID \
if (!m_plus) { return; }

#define JNI_PLUS_CHECK_JSTRING \
if (!m_plus) { return env->NewStringUTF("Error!"); }

#define JNI_JOIN(str1, str2) #str1#str2

jstring ToJString(JNIEnv* env, const char * var) { return env->NewStringUTF(var); }
const char * FromJString(JNIEnv* env, jstring var) { return env->GetStringUTFChars(var, NULL); }


////////////////////////////////////////////////////////////////////////////////////////////
// Methods

#define libAndroidMethod(name) \
JNI_HEADER_DO(name) \
  { JNI_PLUS_CHECK_VOID m_plus->Call(PLUSdevice::e_##name); }

#define libAndroidMethod1(name) \
JNI_HEADER_DO1(name) \
  { JNI_PLUS_CHECK_VOID m_plus->Call(PLUSdevice::e_##name, FromJString(env, p1)); }

#define libAndroidMethod2(name) \
JNI_HEADER_DO2(name) \
  { JNI_PLUS_CHECK_VOID m_plus->Call(PLUSdevice::e_##name, FromJString(env, p1), FromJString(env, p2)); }

#define libAndroidMethod3(name) \
JNI_HEADER_DO3(name) \
  { JNI_PLUS_CHECK_VOID m_plus->Call(PLUSdevice::e_##name, FromJString(env, p1), FromJString(env, p2), FromJString(env, p3)); }

#define libAndroidMediaMethod(name) \
JNI_HEADER_MEDIA(name) \
  { JNI_PLUS_CHECK_VOID m_plus->in##name(void * data, int size); }



// Initialise/Unitialise
JNI_HEADER(Load) { 
    if (!m_plus) {
        m_plus = new JNI_PLUSdevice(env, thiz);
        do {
            usleep(100);
        } while (m_plus->IsLoading());
    }
}

JNI_HEADER(UnLoad) { 
    if (m_plus) { 
        delete m_plus; 
        m_plus = NULL; 
    } 
}

libAndroidMethod1(placeCall)
libAndroidMethod(hangupCall)
libAndroidMethod(answerCall)
libAndroidMethod1(sendDTMF)
libAndroidMethod3(farEndCamera)
libAndroidMethod(start)
libAndroidMethod3(farEndMotorized)
libAndroidMethod1(realTimeTextInput)
libAndroidMethod(realTimeTextNewLine)
libAndroidMethod1(secondCall)
libAndroidMethod(stop)
libAndroidMethod3(videosize)
libAndroidMethod(dhParameters)
// IMPL: Method Names Here

///////////////////////////////////////////////////////////////////////////////////////////
// Media

#define libAndroidMedia(name) \
JNI_HEADER_MEDIA(name) \
{ \
    JNI_PLUS_CHECK_VOID \
    jbyte* bufferPtr = env->GetByteArrayElements(data, NULL); \
    void *inputBytes = malloc(size * sizeof(char)); \
    memcpy(inputBytes, bufferPtr, size); \
    env->ReleaseByteArrayElements(data, bufferPtr, 0); \
    m_plus->in##name(inputBytes, size, width, height); \
    return; \
}

libAndroidMedia(audio)
libAndroidMedia(video)
libAndroidMedia(content)

///////////////////////////////////////////////////////////////////////////////////////////
// Settings

#define libAndroidSetting(name) \
JNI_HEADER_GETJSTRING(name) \
  { JNI_PLUS_CHECK_JSTRING return ToJString(env, m_plus->Get_Value(PLUSdevice::e_##name)); } \
JNI_HEADER_SETVOID(name) \
  { JNI_PLUS_CHECK_VOID m_plus->Set_Value(PLUSdevice::e_##name, FromJString(env, value)); }

libAndroidSetting(version)
libAndroidSetting(tracing)
libAndroidSetting(username)
libAndroidSetting(password)
libAndroidSetting(server)
libAndroidSetting(quality)
libAndroidSetting(accessability)
libAndroidSetting(content)
libAndroidSetting(autoanswer)

libAndroidSetting(drvvideoplay)
libAndroidSetting(drvvideorec)
libAndroidSetting(drvaudioplay)
libAndroidSetting(drvaudiorec)
libAndroidSetting(curdrvvideoplay)
libAndroidSetting(curdrvvideorec)
libAndroidSetting(curdrvaudioplay)
libAndroidSetting(curdrvaudiorec)
libAndroidSetting(devvideoplay)
libAndroidSetting(devvideorec)
libAndroidSetting(devaudioplay)
libAndroidSetting(devaudiorec)
libAndroidSetting(audioplay)
libAndroidSetting(audiorec)
libAndroidSetting(videoplay)
libAndroidSetting(videorec)

libAndroidSetting(call)
libAndroidSetting(audiomute)
libAndroidSetting(videomute)
//libAndroidSetting(framewidth)
//libAndroidSetting(frameheight)
libAndroidSetting(callstate)
libAndroidSetting(farEndCameraSupport)
libAndroidSetting(realTimeTextSupport)
libAndroidSetting(farEndMotorizedSupport)
libAndroidSetting(farEndCameraInCall)
libAndroidSetting(realTimeTextInCall)
libAndroidSetting(farEndMotorizedInCall)
libAndroidSetting(userPhoneNumber)
libAndroidSetting(userURIAddress)
libAndroidSetting(showlocalvideo)
//libAndroidSetting(instanceid)  - Not required 
libAndroidSetting(initialised)
libAndroidSetting(language)
libAndroidSetting(listenport)
libAndroidSetting(videoformats)
libAndroidSetting(videoinformat)
libAndroidSetting(videooutformat)
libAndroidSetting(secondvideo)

libAndroidSetting(encryptSignal)
libAndroidSetting(encryptMedia)
libAndroidSetting(encryptMediaHigh)
// IMPL: Setting Names Here

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Events

#define JNI_EVENTMAP1(name) \
    mid = env->GetMethodID(cls, JNI_JOIN(on,name), "(Ljava/lang/String;)V"); \
    if (mid != NULL) \
        m_eventMap.insert(std::make_pair(PLUSdevice::e_##name,mid));

#define JNI_EVENTMEDIA(name) \
    mid = env->GetMethodID(cls, JNI_JOIN(out,name), "([BI)V"); \
    if (mid != NULL) \
        m_mediaMap.insert(std::make_pair(PLUSdevice::e_##name##Out,mid));


#define PLUSMediaBody(name) \
    bool JNI_PLUSdevice::out##name(const void * data, int size, int /*width*/, int /*height*/) \
    { \
        jmethodID mid = m_mediaMap[PLUSdevice::e_##name##Out]; \
        if (mid != 0) { \
            jbyteArray result = m_env->NewByteArray(size); \
            m_env->SetByteArrayRegion(result, 0, size, (const jbyte*)data); \
            m_env->CallVoidMethod(m_obj, mid, result, size); \
        } \
        return true; \
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
JNI_PLUSdevice::JNI_PLUSdevice(JNIEnv* env, jobject obj)
: m_env(env), m_obj(obj)
{
    jclass cls = env->GetObjectClass(obj);
    jmethodID mid;

    // Load the event map
    JNI_EVENTMAP1(status)
    JNI_EVENTMAP1(isinitialised)
    //JNI_EVENTMAP1(notused1)
    JNI_EVENTMAP1(callerid)
    JNI_EVENTMAP1(incomingcall)
    JNI_EVENTMAP1(incall)
    JNI_EVENTMAP1(encryption)
    JNI_EVENTMAP1(farEndCameraInstruct)
    JNI_EVENTMAP1(realTimeTextInstruct)
    JNI_EVENTMAP1(farEndMotoisedInstruct)
    JNI_EVENTMAP1(PhoneNumberEvent)
    JNI_EVENTMAP1(URIaddressEvent)
    JNI_EVENTMAP1(presenceSupport)
    JNI_EVENTMAP1(duplicate)
    JNI_EVENTMAP1(forwardCall)
    JNI_EVENTMAP1(dhGenerate)
    // IMPL: Event Names here

    JNI_EVENTMEDIA(audio)
    JNI_EVENTMEDIA(video)
    JNI_EVENTMEDIA(content)

}

JNI_PLUSdevice::~JNI_PLUSdevice()
{
    m_eventMap.clear();
}

void JNI_PLUSdevice::Event(int evtID, const char * p1, const char * p2, const char * p3, const char * p4)
{
    jmethodID mid = m_eventMap[evtID];
    if (mid != 0)
        m_env->CallVoidMethod(m_obj, mid, m_env->NewStringUTF(p1));
}

PLUSMediaBody(audio)

PLUSMediaBody(video)

PLUSMediaBody(content)


