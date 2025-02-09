#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "lvgl.h"
#include "../src/ui/ui.h"
#include "ThreadTask.h"
#include <Adafruit_TSC2007.h>

#define DRAW_BUF_SIZE (TFT_WIDTH * TFT_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
#define TOUCH_ADDR 0x48

#define DISPLAY_MESSAGE_TITLE 1
#define DISPLAY_MESSAGE_ARTIST 2
#define DISPLAY_MESSAGE_STATION 3
#define DISPLAY_MESSAGE_WIFI_CONNECTED 4
#define DISPLAY_MESSAGE_WIFI_DISCONNECTED 5
#define DISPLAY_MESSAGE_COMMERCIAL 6
#define DISPLAY_MESSAGE_ERROR 7
#define DISPLAY_MESSAGE_TUNE_UP 8
#define DISPLAY_MESSAGE_TUNE_DOWN 9
#define DISPLAY_MESSAGE_UPDATE_CLOCK 11


#define DISPLAY_MESSAGE_SCREEN_RADIO 20
#define DISPLAY_MESSAGE_SCREEN_CLOCK 21
#define DISPLAY_MESSAGE_SCREEN_STATION_SELECTION 22
#define DISPLAY_MESSAGE_SCREEN_TUNE 23 
#define DISPLAY_MESSAGE_SCREEN_NETWORK 24
#define DISPLAY_MESSAGE_SCREEN_MODE 25
#define DISPLAY_MESSAGE_SCREEN_LOADING 26
#define DISPLAY_MESSAGE_SCREEN_MESSAGE 27
#define DISPLAY_MESSAGE_SCREEN_MESSAGE_MESSAGE 28
#define DISPLAY_MESSAGE_UPDATE_STATIONS 29

#define DISPLAY_MAX_STATION_UI_LIST 50

class DisplayTask : public ThreadTask {

public:
    DisplayTask();
    static bool begin();
    lv_indev_t * indev_touchpad;
    void tick();
    static void readTouchCB(lv_indev_t *device, lv_indev_data_t *data);
    volatile bool touchStarted = false;
    static volatile bool fireTouchRead;
    static volatile bool disableInterrupt;
    static void IRAM_ATTR touchISR();
    lv_obj_t * getActiveScreen();

private:
    bool start();
    SemaphoreHandle_t mutex;
    uint32_t draw_buf[DRAW_BUF_SIZE / 4];
    Adafruit_TSC2007 touchController;
    bool readTouch(uint16_t* x,uint16_t* y,uint16_t* z1,uint16_t* z2);  
    lv_display_t * display;
    void showScreen(uint16_t screenId);

};

extern DisplayTask Display;


