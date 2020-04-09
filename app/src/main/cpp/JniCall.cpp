//
// Created by Lex on 2019-11-22.
//

#include "JniCall.h"

JniCall::JniCall(JavaVM *javaVm, JNIEnv *jniEnv,jobject jPlayerObj) {
    this->javaVm = javaVm;
    this->jniEnv = jniEnv;
    this->jPlayerObj = jPlayerObj;
    initCreateAudioTrack();
    jclass jPlayerClass = jniEnv->GetObjectClass(jPlayerObj);
    jPlayerErrorMid = jniEnv->GetMethodID(jPlayerClass, "onError", "(ILjava/lang/String;)V");

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
                                       sampleRateInHz, channelConfig, audioFormat, bufferSizeInBytes, mode);

    jmethodID playMid = jniEnv->GetMethodID(jAudioTrackClass,"play","()V");
    jAudioTrackPause = jniEnv->GetMethodID(jAudioTrackClass,"pause","()V");
    jniEnv->CallVoidMethod(jAudioTrackObj,playMid);

    jAudioTrackWriteMid = jniEnv->GetMethodID(jAudioTrackClass,"write","([BII)I");
}

void JniCall::callAudioTrackWrite(jbyteArray audioData, int offsetInBytes, int sizeInBytes) {
    jniEnv->CallIntMethod(jAudioTrackObj,jAudioTrackWriteMid,audioData,offsetInBytes,sizeInBytes);
}

void JniCall::pause() {
    jniEnv->CallVoidMethod(jAudioTrackObj,jAudioTrackPause);
}

JniCall::~JniCall() {
    jniEnv->DeleteGlobalRef(jAudioTrackObj);
}

void JniCall::callPlayerError(int code, char *msg) {
    jstring  jmsg = jniEnv->NewStringUTF(msg);
    jniEnv->CallVoidMethod(jPlayerObj,jPlayerErrorMid,code,jmsg);
    jniEnv->DeleteLocalRef(jmsg);
}
