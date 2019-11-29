#include "Constant.h"
#include "JniCall.h"
#include "FFmpegPlay.h"
#include <string>

// 在 c++ 中采用 c 的这种编译方式
extern "C" {
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
}

JniCall *jniCall;
FFmpegPlay *ffmpegPlay;

extern "C"

JNIEXPORT void JNICALL
Java_com_ldlywt_ffmpegstudy_Player_playMusic(JNIEnv *env, jobject thiz, jstring url_) {
    const char *url = env->GetStringUTFChars(url_, 0);
    jniCall = new JniCall(nullptr, env,thiz);
    ffmpegPlay = new FFmpegPlay(jniCall, url);
    ffmpegPlay->play();
//    delete jniCall;
//    delete ffmpegPlay;
    env->ReleaseStringUTFChars(url_, url);
}