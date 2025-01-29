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
#define DISPLAY_MESSAGE_START 6
#define DISPLAY_MESSAGE_COMMERCIAL 7
#define DISPLAY_MESSAGE_ERROR 8
#define DISPLAY_MESSAGE_TTIME 9
#define DISPLAY_MESSAGE_STATION_LIST 10
#define DISPLAY_MESSAGE_WIFI_CONNECTING 11
#define DISPLAY_MESSAGE_FIND_IPADDRESS 12
#define DISPLAY_MESSAGE_IPADDRESS 13
#define DISPLAY_MESSAGE_FIND_TIMEZONE 14
#define DISPLAY_MESSAGE_TIMEZONE 15
#define DISPLAY_MESSAGE_TUNE_UP 16
#define DISPLAY_MESSAGE_TUNE_DOWN 17
#define DISPLAY_MESSAGE_SELECT 18


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

    void showMessageUI(const char * message);
    void showRadioUI();

};

extern DisplayTask Display;


