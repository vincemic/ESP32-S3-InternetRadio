#pragma once

#include <Arduino.h>
#include <Audio.h>
#include "ThreadTask.h"

//#define RADIO_STREAM "http://legacy.scahw.com.au/2classicrock_32"
//#define RADIO_STREAM "http://stream.srg-ssr.ch/m/rsp/mp3_128"
//#define RADIO_STREAM "http://www.radioeins.de/frankfurt/livemp3"
//#define RADIO_STREAM "http://vis.media-ice.musicradio.com/CapitalMP3"
//#define RADIO_STREAM "http://rfcm.streamguys1.com/thirdrock-icy"
#define RADIO_STREAM "http://stream.laut.fm/oldies"
class SoundTask : public ThreadTask {

public:
    SoundTask();
    static bool begin();
    void setVolume(uint8_t vol);
    uint8_t getVolume();

    void tick();

private:
    uint16_t volume = 0;
    bool start();
    void turnUpVolume();
    void turnDownVolume();
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
#define SOUND_MESSAGE_TURN_UP_VOLUME 12
#define SOUND_MESSAGE_TURN_DOWN_VOLUME 13
#define SOUND_MESSAGE_CONNECT 14
