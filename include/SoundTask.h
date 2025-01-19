#pragma once

#include <Arduino.h>
#include <Audio.h>
#include "ThreadTask.h"

class SoundTask : public ThreadTask {

public:
    SoundTask();
    bool init();
    void setVolume(uint8_t vol);
    uint8_t getVolume();
    bool connecttohost(const char* host);
    bool connecttoSD(const char* filename);
    Audio audio;

};

extern SoundTask Sound;

#define SOUND_MESSAGE_id3data 1
#define SOUND_MESSAGE_eof_mp3 2
#define SOUND_MESSAGE_showstation 3
#define SOUND_MESSAGE_showstreaminfo 4
#define SOUND_MESSAGE_showstreamtitle 5
#define SOUND_MESSAGE_bitrate 6
#define SOUND_MESSAGE_commercial 7
#define SOUND_MESSAGE_icyurl 8
#define SOUND_MESSAGE_lasthost 9
#define SOUND_MESSAGE_eof_speech 10
#define SOUND_MESSAGE_icylogo 11
