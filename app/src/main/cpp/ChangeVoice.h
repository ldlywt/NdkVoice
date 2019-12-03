//
// Created by Lex on 2019-12-02.
//

#ifndef FFMPEGSTUDY_CHANGEVOICE_H
#define FFMPEGSTUDY_CHANGEVOICE_H

// 定义五种不同的模式
#include <inc/fmod.hpp>

#define MODE_NORMAL 0
#define MODE_LUOLI 1
#define MODE_DASHU 2
#define MODE_JINGSONG 3
#define MODE_GAOGUAI 4
#define MODE_KONGLING 5


class ChangeVoice {
public:
    JNIEnv *env;
    const char *url;
    int mode;
    FMOD::System *system;
public:
    ChangeVoice(JNIEnv *env, const char *url, int mode);

    ~ChangeVoice();

public:
    void play();

    void playVoice();
};


#endif //FFMPEGSTUDY_CHANGEVOICE_H
