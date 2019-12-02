//
// Created by Lex on 2019-11-25.
//

#include "FFmpegPlay.h"

FFmpegPlay::FFmpegPlay(JniCall *jniCall, const char *url) {
    this->jniCall = jniCall;
    this->url = url;
}

FFmpegPlay::~FFmpegPlay() {
    release();
}

void FFmpegPlay::release() {
    if (this->avCodecContext != nullptr) {
        avcodec_close(this->avCodecContext);
        avcodec_free_context(&this->avCodecContext);
        this->avCodecContext = nullptr;
    }

    if (this->avFormatContext != nullptr) {
        avformat_close_input(&this->avFormatContext);
        avformat_free_context(this->avFormatContext);
        this->avFormatContext = nullptr;
    }

    if (this->swrContext != nullptr) {
        swr_free(&this->swrContext);
        free(this->swrContext);
        this->swrContext = nullptr;
    }

    if (this->resampleOutBuffer != nullptr) {
        free(this->resampleOutBuffer);
        this->resampleOutBuffer = nullptr;
    }

    avformat_network_deinit();
}

void FFmpegPlay::play() {
    ///1
    av_register_all();

    avformat_network_init();

    /// 2
    int openInputCode = avformat_open_input(&avFormatContext, url, nullptr, nullptr);
    if ( openInputCode != 0) {
        LOGI("步骤二失败");
        callPlayerJniError(openInputCode, av_err2str(openInputCode));
        return;
    }



    /// 3
    if (avformat_find_stream_info(avFormatContext, nullptr) < 0) {
        LOGI("步骤三失败");
        return;
    }



    /// 4
    int audioStreamIndex = -1;
    audioStreamIndex = av_find_best_stream(avFormatContext, AVMediaType::AVMEDIA_TYPE_AUDIO, -1, -1,
                                           nullptr, 0);
    if (audioStreamIndex < 0) {
        LOGI("步骤四失败");
        return;
    }

    /// 5 查找解码器
    AVCodecParameters *avCodecParameters = avFormatContext->streams[audioStreamIndex]->codecpar;
    AVCodecID codecId = avCodecParameters->codec_id;
    AVCodec *avCodec = avcodec_find_decoder(codecId);
    if (avCodec == nullptr) {
        LOGI("步骤五失败");
        return;
    }

    /// 6 打开解码器
    //申请AVCodecContext内存
    avCodecContext = avcodec_alloc_context3(avCodec);
    //将AVCodecContext赋值
    if (avcodec_parameters_to_context(avCodecContext, avCodecParameters) < 0) {
        LOGI("error");
        return;
    }

    if (avcodec_open2(avCodecContext, avCodec, nullptr) != 0) {
        LOGI("步骤六失败");
        return;
    }

    // ---------- 重采样 start ----------
    int64_t out_ch_layout = AV_CH_LAYOUT_STEREO;
    enum AVSampleFormat out_sample_fmt = AVSampleFormat::AV_SAMPLE_FMT_S16;
    int out_sample_rate = AUDIO_SAMPLE_RATE;
    int64_t in_ch_layout = avCodecContext->channel_layout;
    enum AVSampleFormat in_sample_fmt = avCodecContext->sample_fmt;
    int in_sample_rate = avCodecContext->sample_rate;

    swrContext = swr_alloc_set_opts(nullptr, out_ch_layout, out_sample_fmt,
                                    out_sample_rate, in_ch_layout, in_sample_fmt,
                                    in_sample_rate, 0, nullptr);
    if (swrContext == nullptr) {
        LOGI("error");
    }

    int swrInitRes = swr_init(swrContext);
    if (swrInitRes < 0) { return; }

    int outChannels = av_get_channel_layout_nb_channels(out_ch_layout);
    int dataSize = av_samples_get_buffer_size(nullptr, outChannels,
                                              avCodecParameters->frame_size, out_sample_fmt, 0);
    resampleOutBuffer = (uint8_t *) malloc(dataSize);
    // ---------- 重采样 end ----------

    jbyteArray jPcmByteArray = jniCall->jniEnv->NewByteArray(dataSize);
    jbyte *jPcmData = jniCall->jniEnv->GetByteArrayElements(jPcmByteArray, nullptr);

    AVPacket *avPacket = av_packet_alloc();
    AVFrame *avFrame = av_frame_alloc();

    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        if (avPacket->stream_index == audioStreamIndex) {
            ///向解码器发送数据
            int codecSendPacketRes = avcodec_send_packet(avCodecContext, avPacket);
            if (codecSendPacketRes == 0) {
                ///从解码器接收数据
                int codecReceiveFrameRes = avcodec_receive_frame(avCodecContext, avFrame);
                if (codecReceiveFrameRes == 0) {

                    // 调用重采样的方法
                    swr_convert(swrContext, &resampleOutBuffer, avFrame->nb_samples,
                                (const uint8_t **) avFrame->data, avFrame->nb_samples);

                    // write 写到缓冲区 pFrame.data -> javabyte
                    // size 是多大，装 pcm 的数据
                    // 1s 44100 点  2通道 ，2字节    44100*2*2
                    // 1帧不是一秒，pFrame->nb_samples点
                    memcpy(jPcmData, resampleOutBuffer, dataSize);
                    // 0 把 c 的数组的数据同步到 jbyteArray , 然后释放native数组
                    jniCall->jniEnv->ReleaseByteArrayElements(jPcmByteArray, jPcmData, JNI_COMMIT);
                    // TODO
                    jniCall->callAudioTrackWrite(jPcmByteArray, 0, dataSize);
                }
                //解引用
                av_packet_unref(avPacket);
                av_frame_unref(avFrame);
            }


        }


    }



    // 1. 解引用数据 data ， 2. 销毁 pPacket 结构体内存  3. pPacket = NULL
    av_packet_free(&avPacket);
    av_frame_free(&avFrame);
    // 解除 jPcmDataArray 的持有，让 javaGC 回收
    jniCall->jniEnv->ReleaseByteArrayElements(jPcmByteArray, jPcmData, 0);
    jniCall->jniEnv->DeleteLocalRef(jPcmByteArray);

}

void FFmpegPlay::callPlayerJniError(int code, char *msg) {
    release();
    // 回调给 java 层调用
    jniCall->callPlayerError(code, msg);

}

void FFmpegPlay::prepare() {

}
