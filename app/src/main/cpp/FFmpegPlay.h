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
    JniCall *jniCall = nullptr;

public:

    FFmpegPlay(JniCall *jniCall);

    ~FFmpegPlay();

public:

    void setDataSource(const char *path);

    void play();

    void pause();

    void prepare();

    void callPlayerJniError(int code, char *url);

    void release();

private:
    pthread_t prepareTask;
    char *url;

};


#endif //FFMPEGSTUDY_FFMPEGPLAY_H
