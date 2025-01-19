#pragma once
#include <Arduino.h>
#include "ThreadTask.h"
#include <Adafruit_seesaw.h>
#include <seesaw_neopixel.h>
#include <Adafruit_TSC2007.h>

#define SS_SWITCH        24
#define SS_NEOPIX        6

#define ROTARY_SEESAW_ADDR          0x36
#define TOUCH_SEESAW_ADDR           0x48
//#define SCL 9
//#define SDA1 8




class DeviceTask : public ThreadTask {      

public:
    DeviceTask();
    bool init();
    uint32_t readRotarySwitch();
    uint32_t readRotaryPostion();
    bool readTouch(uint16_t* x,uint16_t* y,uint16_t* z1,uint16_t* z2);
    uint32_t color(uint8_t r, uint8_t g, uint8_t b);
    void setRotaryPixelColor(uint16_t n, uint32_t c);
 private:   
    Adafruit_seesaw rotary_seesaw;
    Adafruit_TSC2007 touch_seesaw; 
    seesaw_NeoPixel sspixel;
    int32_t encoder_position;
};

extern DeviceTask Device;