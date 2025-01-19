#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "lvgl.h"
#include "../src/ui/ui.h"
#include "ThreadTask.h"

class DisplayTask : public ThreadTask {

public:
    DisplayTask();
    bool init();
   

};

extern DisplayTask Display;


#define DISPLAY_MESSAGE_TITLE 1
#define DISPLAY_MESSAGE_ARTIST 2
#define DISPLAY_MESSAGE_STATION 3