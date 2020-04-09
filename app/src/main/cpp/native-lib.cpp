#include "Constant.h"
#include "JniCall.h"
#include "FFmpegPlay.h"
#include <string>
#include "ChangeVoice.h"


extern "C"
JNIEXPORT jlong JNICALL
Java_com_ldlywt_ffmpegstudy_NativeUtil_nativeInit(JNIEnv *env, jobject thiz) {
    JniCall *jniCall = new JniCall(nullptr, env, thiz);
    FFmpegPlay *ffmpegPlay = new FFmpegPlay(jniCall);
    return reinterpret_cast<jlong>(ffmpegPlay);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ldlywt_ffmpegstudy_NativeUtil_prepare(JNIEnv *env, jobject thiz, jlong native_handle) {


}

extern "C" JNIEXPORT void JNICALL
Java_com_ldlywt_ffmpegstudy_NativeUtil_playMusic(JNIEnv *env, jobject thiz,jlong native_handle, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, nullptr);
    FFmpegPlay *play = reinterpret_cast<FFmpegPlay *>(native_handle);
    play->setDataSource(url);
    play->play();
    env->ReleaseStringUTFChars(url_, url);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ldlywt_ffmpegstudy_NativeUtil_pauseMusic(JNIEnv *env, jobject thiz) {
//    jniCall = new JniCall(nullptr, env, thiz);
//    jniCall->pause();
}

ChangeVoice *changeVoice;
extern "C" JNIEXPORT void JNICALL
Java_com_ldlywt_ffmpegstudy_NativeUtil_changeVoice(JNIEnv *env, jclass clazz, jstring path,
                                                   jint mode) {
    const char *url = env->GetStringUTFChars(path, nullptr);
    changeVoice = new ChangeVoice(env, url, mode);
    changeVoice->playVoice();
    env->ReleaseStringUTFChars(path, url);
}