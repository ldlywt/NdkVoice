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
    jobject jPlayerObj;

public:
    JniCall(JavaVM *javaVm, JNIEnv *jniEnv,jobject jPlayerObj);

    ~JniCall();

private:
    void initCreateAudioTrack();

public:
    void callAudioTrackWrite(jbyteArray audioData, int offsetInBytes, int sizeInBytes);
    void callPlayerError(int code, char *msg);
    void pause();
};


#endif //FFMPEGSTUDY_JNICALL_H
