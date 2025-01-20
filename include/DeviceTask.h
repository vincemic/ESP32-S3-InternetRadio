#pragma once
#include <Arduino.h>
#include "ThreadTask.h"
#include <Adafruit_seesaw.h>
#include <seesaw_neopixel.h>


#define SS_SWITCH        24
#define SS_NEOPIX        6

#define ROTARY_ADDR          0x36


class DeviceTask : public ThreadTask {      

public:
    DeviceTask();
    bool init();
    uint32_t readRotarySwitch();
    uint32_t readRotaryPostion();
    uint32_t color(uint8_t r, uint8_t g, uint8_t b);
    void setRotaryPixelColor(uint16_t n, uint32_t c);
 
 private: 

    Adafruit_seesaw rotary_seesaw;
    seesaw_NeoPixel sspixel;
    int32_t encoder_position;
};

extern DeviceTask Device;