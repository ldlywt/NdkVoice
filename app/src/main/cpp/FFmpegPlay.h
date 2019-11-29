//
// Created by Lex on 2019-11-25.
//

#ifndef FFMPEGSTUDY_FFMPEGPLAY_H
#define FFMPEGSTUDY_FFMPEGPLAY_H

extern "C" {
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
#include "JniCall.h"
#include <pthread.h>
};

class FFmpegPlay {
public:
    AVFormatContext *avFormatContext = nullptr;
    AVCodecContext *avCodecContext = nullptr;
    SwrContext *swrContext = nullptr;
    uint8_t *resampleOutBuffer = nullptr;
    const char *url = nullptr;
    JniCall *jniCall = nullptr;

public:
    FFmpegPlay(JniCall *jniCall, const char *url);

    ~FFmpegPlay();

public:
    void play();

    void prepare();

    void callPlayerJniError(int code, char *url);

    void release();

};


#endif //FFMPEGSTUDY_FFMPEGPLAY_H
