#pragma once
#include <Arduino.h>
#include "ThreadTask.h"


#define SS_SWITCH        24
#define SS_NEOPIX        6

#define ROTARY_SEESAW_ADDR          0x36
#define TOUCH_SEESAW_ADDR           0x48
#define SCL1 9
#define SDA1 8




class DeviceTask : public ThreadTask {      

public:
    DeviceTask();
    bool init();
 private:   
   // Adafruit_seesaw rotary_seesaw;
   // Adafruit_seesaw touch_seesaw;

};

extern DeviceTask Device;