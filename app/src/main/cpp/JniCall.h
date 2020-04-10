//
// Created by Lex on 2019-11-21.
//

#ifndef FFMPEGSTUDY_JNICALL_H
#define FFMPEGSTUDY_JNICALL_H


#include <jni.h>
#include "Constant.h"




class JniCall {
public:
    jobject jAudioTrackObj;
    jmethodID jAudioTrackWriteMid;
    jmethodID jAudioTrackPause;
    JavaVM *javaVm;
    JNIEnv *jniEnv;
    jmethodID jPlayerErrorMid;
    jmethodID jPlayerPrepareMid;
    jmethodID jPlayerProgressMid;
    jobject jPlayerObj;

public:
    JniCall(JavaVM *javaVm, JNIEnv *jniEnv, jobject &_jObject);

    ~JniCall();

    void callAudioTrackWrite(jbyteArray audioData, int offsetInBytes, int sizeInBytes);

    void callError(int code, char *msg,int thread = THREAD_MAIN);

    void pause();

    void onPrepare(int thread = THREAD_MAIN);

    void onProgress(int progress,int thread = THREAD_MAIN);

private:
    void initCreateAudioTrack();

};


#endif //FFMPEGSTUDY_JNICALL_H
