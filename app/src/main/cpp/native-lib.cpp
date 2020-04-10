#include "Constant.h"
#include "JniCall.h"
#include "FFmpegPlay.h"
#include <string>
#include "ChangeVoice.h"

JavaVM *javaVM = 0;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    javaVM = vm;
    return JNI_VERSION_1_4;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_ldlywt_ffmpegstudy_NativeUtil_nativeInit(JNIEnv *env, jobject thiz) {
    FFmpegPlay *player = new FFmpegPlay(new JniCall(javaVM, env, thiz));
    return (jlong) player;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ldlywt_ffmpegstudy_NativeUtil_prepare(JNIEnv *env, jobject thiz, jlong native_handle) {


}

extern "C" JNIEXPORT void JNICALL
Java_com_ldlywt_ffmpegstudy_NativeUtil_playMusic(JNIEnv *env, jobject thiz, jstring url_,
                                                 jlong native_handle) {
    const char *url = env->GetStringUTFChars(url_, nullptr);
    FFmpegPlay *play = reinterpret_cast<FFmpegPlay *>(native_handle);
    play->setDataSource(url);
    play->prepare();
    env->ReleaseStringUTFChars(url_, url);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_ldlywt_ffmpegstudy_NativeUtil_pauseMusic(JNIEnv *env, jobject thiz) {

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