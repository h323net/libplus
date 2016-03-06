#include "libPLUS.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "libPLUSjni", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "libPLUSjni", __VA_ARGS__))


#define libAndroidSetting(name) \
extern "C" { \
JNIEXPORT jstring JNICALL Java_com_libPLUSandroid_libPLUSandroid_##name(JNIEnv* env, jobject thiz); \
}; \
JNIEXPORT jstring JNICALL Java_com_libPLUSandroid_libPLUSandroid_##name(JNIEnv* env, jobject thiz) \
{  return env->NewStringUTF("Test");  }

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


extern "C" {
    JNIEXPORT jstring JNICALL Java_com_libPLUSandroid_libPLUSandroid_stringFromJNI(JNIEnv* env, jobject thiz);
}

JNIEXPORT jstring JNICALL Java_com_libPLUSandroid_libPLUSandroid_stringFromJNI(JNIEnv* env, jobject thiz) {
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

