/*
* libPLUS.cpp
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

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "libPLUSjni", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "libPLUSjni", __VA_ARGS__))

PLUSdevice * m_plus = NULL;

// MACROS

#define JNI_PATH Java_com_libPLUSandroid_libPLUSandroid_

#define JNI_HEADER(name) \
extern "C" { \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_##name(JNIEnv* env, jobject thiz); \
}; \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_##name(JNIEnv* env, jobject thiz)

#define JNI_HEADER_VOID(name) \
extern "C" { \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_set##name(JNIEnv* env, jobject thiz, jstring value); \
}; \
JNIEXPORT void JNICALL Java_com_libPLUSandroid_libPLUSandroid_set##name(JNIEnv* env, jobject thiz, jstring value)

#define JNI_HEADER_JSTRING(name) \
extern "C" { \
JNIEXPORT jstring JNICALL Java_com_libPLUSandroid_libPLUSandroid_##name(JNIEnv* env, jobject thiz); \
}; \
JNIEXPORT jstring JNICALL Java_com_libPLUSandroid_libPLUSandroid_##name(JNIEnv* env, jobject thiz)

#define JNI_PLUS_CHECK_VOID \
if (!m_plus) { return; }

#define JNI_PLUS_CHECK_JSTRING \
if (!m_plus) { return env->NewStringUTF("Error!"); }

jstring ToJString(JNIEnv* env, std::string var)
{
    return env->NewStringUTF(var.c_str());
}

#define libAndroidSetting(name) \
JNI_HEADER_JSTRING(name) \
  { JNI_PLUS_CHECK_JSTRING return ToJString(env, m_plus->Get_Value(PLUSdevice::e_##name)); } \
JNI_HEADER_VOID(name) \
  { JNI_PLUS_CHECK_VOID m_plus->Set_Value(PLUSdevice::e_##name, env->GetStringUTFChars(value, NULL)); }


// Initialise/Unitialise
JNI_HEADER(Load) { if (!m_plus)  m_plus = new PLUSdevice(); }
JNI_HEADER(UnLoad) { if (m_plus) { delete m_plus; m_plus = NULL; } }

// Settings
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
libAndroidSetting(encryptSignal)
libAndroidSetting(encryptMedia)
libAndroidSetting(encryptMediaHigh)
// IMPL: Setting Name here


JNI_HEADER_JSTRING(stringFromJNI)
{
#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
#if defined(__ARM_PCS_VFP)
#define ABI "armeabi-v7a/NEON (hard-float)"
#else
#define ABI "armeabi-v7a/NEON"
#endif
#else
#if defined(__ARM_PCS_VFP)
#define ABI "armeabi-v7a (hard-float)"
#else
#define ABI "armeabi-v7a"
#endif
#endif
#else
#define ABI "armeabi"
#endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__x86_64__)
#define ABI "x86_64"
#elif defined(__mips64)  // mips64el-* toolchain defines __mips__ too
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif

    return env->NewStringUTF("Hello from JNI !  Compiled with ABI " ABI ".");
}

