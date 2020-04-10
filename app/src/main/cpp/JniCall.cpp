//
// Created by Lex on 2019-11-22.
//

#include "JniCall.h"

JniCall::JniCall(JavaVM *javaVm, JNIEnv *jniEnv, jobject &_jObject) : javaVm(javaVm),
                                                                      jniEnv(jniEnv) {
//    this->javaVm = javaVm;
//    this->jniEnv = jniEnv;
//    this->jPlayerObj = jPlayerObj;
    jPlayerObj = jniEnv->NewGlobalRef(_jObject);
    initCreateAudioTrack();
    jclass jPlayerClass = jniEnv->GetObjectClass(jPlayerObj);
    jPlayerErrorMid = jniEnv->GetMethodID(jPlayerClass, "onError", "(ILjava/lang/String;)V");
    jPlayerPrepareMid = jniEnv->GetMethodID(jPlayerClass, "onPrepare", "()V");
    jPlayerProgressMid = jniEnv->GetMethodID(jPlayerClass, "onProgress", "(I)V");

}

void JniCall::initCreateAudioTrack() {
    jclass jAudioTrackClass = jniEnv->FindClass("android/media/AudioTrack");
    jmethodID jAudioTrackMid = jniEnv->GetMethodID(jAudioTrackClass, "<init>", "(IIIIII)V");

    int streamType = 3;
    int sampleRateInHz = AUDIO_SAMPLE_RATE;
    int channelConfig = (0x4 | 0x8);
    int audioFormat = 2;
    int mode = 1;

    jmethodID getMinBufferSizeId = jniEnv->GetStaticMethodID(jAudioTrackClass, "getMinBufferSize",
                                                             "(III)I");
    int bufferSizeInBytes = jniEnv->CallStaticIntMethod(jAudioTrackClass, getMinBufferSizeId,
                                                        sampleRateInHz, channelConfig, audioFormat);
    jAudioTrackObj = jniEnv->NewObject(jAudioTrackClass, jAudioTrackMid, streamType,
                                       sampleRateInHz, channelConfig, audioFormat,
                                       bufferSizeInBytes, mode);

    jmethodID playMid = jniEnv->GetMethodID(jAudioTrackClass, "play", "()V");
    jAudioTrackPause = jniEnv->GetMethodID(jAudioTrackClass, "pause", "()V");
    jniEnv->CallVoidMethod(jAudioTrackObj, playMid);

    jAudioTrackWriteMid = jniEnv->GetMethodID(jAudioTrackClass, "write", "([BII)I");
}

void JniCall::callAudioTrackWrite(jbyteArray audioData, int offsetInBytes, int sizeInBytes) {
    jniEnv->CallIntMethod(jAudioTrackObj, jAudioTrackWriteMid, audioData, offsetInBytes,
                          sizeInBytes);
}

void JniCall::pause() {
    jniEnv->CallVoidMethod(jAudioTrackObj, jAudioTrackPause);
}

JniCall::~JniCall() {
    jniEnv->DeleteGlobalRef(jPlayerObj);
    jniEnv->DeleteGlobalRef(jAudioTrackObj);
}

void JniCall::callError(int code, char *msg, int thread) {
    if (thread == THREAD_CHILD) {
        //子线程，JNIEnv是每个线程独有的
        JNIEnv *env;
        if (javaVm->AttachCurrentThread(&env, 0) != JNI_OK) {
            return;
        }
        jstring jmsg = env->NewStringUTF(msg);
        env->CallVoidMethod(jPlayerObj, jPlayerErrorMid, code, jmsg);
        env->DeleteLocalRef(jmsg);
        javaVm->DetachCurrentThread();
    } else {
        jstring jmsg = jniEnv->NewStringUTF(msg);
        jniEnv->CallVoidMethod(jPlayerObj, jPlayerErrorMid, code, jmsg);
        jniEnv->DeleteLocalRef(jmsg);
    }

}

void JniCall::onPrepare(int thread) {
    if (thread == THREAD_CHILD) {
        JNIEnv *env;
        if (javaVm->AttachCurrentThread(&env, 0) != JNI_OK) {
            return;
        }
        env->CallVoidMethod(jPlayerObj, jPlayerPrepareMid);
        javaVm->DetachCurrentThread();
    } else {
        jniEnv->CallVoidMethod(jPlayerObj, jPlayerPrepareMid);
    }
}

void JniCall::onProgress(int progress, int thread) {
    if (thread == THREAD_CHILD) {
        JNIEnv *env;
        if (javaVm->AttachCurrentThread(&env, 0) != JNI_OK) {
            return;
        }
        env->CallVoidMethod(jPlayerObj,jPlayerProgressMid,progress);
        javaVm->DetachCurrentThread();
    } else {
        jniEnv->CallVoidMethod(jPlayerObj,jPlayerProgressMid,progress);
    }
}
