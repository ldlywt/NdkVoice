
#include <jni.h>
#include <inc/fmod.hpp>
#include <unistd.h>
#include "ChangeVoice.h"
#include "Constant.h"

ChangeVoice::ChangeVoice(JNIEnv *env, const char *url, int mode) {
    this->env = env;
    this->url = url;
    this->mode = mode;
}

ChangeVoice::~ChangeVoice() {
    system->close();
    system->release();
}

using namespace FMOD;

void ChangeVoice::playVoice() {
    //初始化fmod
    FMOD::System_Create(&system);
    Sound *sound;
    // 通道（声音是由多种音效组成）
    Channel *channel;
    // 音频
    DSP *pDSP;
    // 速度
    float frequency;
    system->init(32, FMOD_INIT_NORMAL, nullptr);
    system->createSound(url, FMOD_DEFAULT, nullptr, &sound);


    try {
        switch (mode) {
            case MODE_NORMAL:
                LOGI("%s", "正常");
                system->playSound(sound, NULL, false, &channel);
                break;
            case MODE_DASHU:
                LOGI("%s", "大叔");
                // 设置音调，调低音调
                system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pDSP);
                pDSP->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.7);
                system->playSound(sound, NULL, false, &channel);
                channel->addDSP(0, pDSP);
                break;
            case MODE_LUOLI:
                LOGI("%s", "萝莉");
                // 设置音调，调高音调
                system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &pDSP);
                pDSP->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 3);
                system->playSound(sound, NULL, false, &channel);
                channel->addDSP(0, pDSP);
                break;
            case MODE_GAOGUAI:
                LOGI("%s", "搞怪");
                system->createDSPByType(FMOD_DSP_TYPE_NORMALIZE, &pDSP);
                system->playSound(sound, NULL, false, &channel);
                channel->addDSP(0, pDSP);
                // 获取速度并加速
                channel->getFrequency(&frequency);
                frequency = frequency * 1.6;
                channel->setFrequency(frequency);
                break;
            case MODE_JINGSONG:
                // FMOD_DSP_TYPE_TREMOLO 颤抖
                LOGI("%s", "惊悚");
                // 设置颤抖
                system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &pDSP);
                // 设置颤抖的频率
                pDSP->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.8);
                system->playSound(sound, NULL, false, &channel);
                channel->addDSP(0, pDSP);
                channel->getFrequency(&frequency);
                frequency = frequency * 0.5;
                channel->setFrequency(frequency);
                break;
            case MODE_KONGLING:
                LOGI("%s", "空灵");
                // 设置重复
                system->createDSPByType(FMOD_DSP_TYPE_ECHO, &pDSP);
                // 设置重复的重复延迟
                pDSP->setParameterFloat(FMOD_DSP_ECHO_DELAY, 300);
                pDSP->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 20);
                system->playSound(sound, NULL, false, &channel);
                channel->addDSP(0, pDSP);
                break;
        }

        system->update();
        bool isPlaying = true;
        // 需要等待，等声音全部播放完成
        // 检查是否播放完成
        while (isPlaying) {
            channel->isPlaying(&isPlaying);
            usleep(1000);
        }
    } catch (...) {
        LOGI("%s", "error");
    }
}



