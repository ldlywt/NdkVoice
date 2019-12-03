#include "Constant.h"
#include "JniCall.h"
#include "FFmpegPlay.h"
#include <string>
#include "ChangeVoice.h"

JniCall *jniCall;
FFmpegPlay *ffmpegPlay;

extern "C" JNIEXPORT void JNICALL
Java_com_ldlywt_ffmpegstudy_NativeUtil_playMusic(JNIEnv *env, jobject thiz, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, nullptr);
    jniCall = new JniCall(nullptr, env, thiz);
    ffmpegPlay = new FFmpegPlay(jniCall, url);
    ffmpegPlay->play();
//    delete jniCall;
//    delete ffmpegPlay;
    env->ReleaseStringUTFChars(url_, url);
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
