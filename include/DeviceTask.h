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
    static bool begin();
    void tick();
    static void IRAM_ATTR rotaryISR();
    static volatile bool fireRotaryRead;
    static volatile bool disableInterrupt;
 private: 
    bool start();
    bool readRotarySwitch();
    uint32_t readRotaryPostion();
    Adafruit_seesaw rotary_seesaw;
    int32_t encoderPosition = 0;
    bool rotarySwitchBounce = true;
    uint8_t isrCounter = 0;
};

extern DeviceTask Device;