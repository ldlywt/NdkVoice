#include "Constant.h"
#include "JniCall.h"
#include "FFmpegPlay.h"
#include <string>
#include <unistd.h>
#include "fmod.hpp"
#include "ChangeVoice.h"

// 在 c++ 中采用 c 的这种编译方式
extern "C" {
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
}

JniCall *jniCall;
FFmpegPlay *ffmpegPlay;

extern "C" JNIEXPORT void JNICALL
Java_com_ldlywt_ffmpegstudy_NativeUtil_playMusic(JNIEnv *env, jobject thiz, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);
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
