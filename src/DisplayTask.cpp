#include "DisplayTask.h"
#include "SoundTask.h"
#include <ArduinoLog.h>

//#define RADIO_STREAM "http://legacy.scahw.com.au/2classicrock_32"
//#define RADIO_STREAM "http://stream.srg-ssr.ch/m/rsp/mp3_128"
//#define RADIO_STREAM "http://www.radioeins.de/frankfurt/livemp3"
//#define RADIO_STREAM "http://vis.media-ice.musicradio.com/CapitalMP3"
//#define RADIO_STREAM "http://rfcm.streamguys1.com/thirdrock-icy"
#define RADIO_STREAM "http://stream.laut.fm/oldies"

static ulong getMills() { return millis(); };


DisplayTask::DisplayTask() {
  
 
}


bool DisplayTask::init()
{

    lv_init();

    // Initialize the TFT display using the TFT_eSPI library
    display = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(display, LV_DISPLAY_ROTATION_270);

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(getMills);
    
    /*Register a touchpad input device*/
    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, readTouchCB);

    ui_init();

    lv_obj_add_flag(uic_Main_Screen_Commercial, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Main_Screen_Artist, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(uic_Main_Screen_Title, LV_OBJ_FLAG_HIDDEN);

    while(!touchController.begin(TOUCH_ADDR))
    { 
        tick();
        delay(500);
    }

    touchStarted = true;
    Log.infoln("Touch controller started");
       
    return true;
}
   
void DisplayTask::readTouchCB(lv_indev_t *device, lv_indev_data_t *data)
{
    static uint16_t highX = 0;
    static uint16_t highY = 0;

    uint16_t x,y,z1,z2;
    int32_t x_diff, y_diff;

    if(Display.readTouch(&x, &y, &z1, &z2)&& z1 > 10)
    {
        Log.infoln("Touch point: (%d, %d)", x, y);
    
        if(x > highX) highX = x;
        if(y > highY) highY = y;

        if(y < 337) y = 337;
        if(x < 337) x = 337;

        if(y > 3808) y = 3808;
        if(x > 3808) x = 3808;

        y_diff = map(y, 337, 3808, 1,  TFT_HEIGHT - 1);
        x_diff= map(x, 337, 3808, 1, TFT_WIDTH - 1);

        data->point.x = x_diff;
        data->point.y = y_diff;

        Log.infoln("Result point: (%d, %d)", x_diff, y_diff);

        if(z1 > 100)
            data->state = LV_INDEV_STATE_PRESSED;
        else
            data->state = LV_INDEV_STATE_RELEASED;

    } else {
        data->point.x = 1;
        data->point.y = 1;
        data->state = LV_INDEV_STATE_RELEASED;

    }
}

bool DisplayTask::readTouch(uint16_t* x,uint16_t* y,uint16_t* z1,uint16_t* z2)
{
    return touchController.read_touch(x,y,z1,z2);
}

void DisplayTask::tick()
{

    ThreadMessage threadMessage;   

    if(internalReceive(&threadMessage)) 
    {
        switch(threadMessage.messageType)
        {
            case DISPLAY_MESSAGE_ARTIST:
                lv_label_set_text(ui_Main_Screen_Artist, threadMessage.message);
                lv_obj_remove_flag(ui_Main_Screen_Artist, LV_OBJ_FLAG_HIDDEN);
                break;
            case DISPLAY_MESSAGE_TITLE:
                lv_label_set_text(ui_Main_Screen_Title, threadMessage.message);
                lv_obj_remove_flag(uic_Main_Screen_Title, LV_OBJ_FLAG_HIDDEN);
                break;
            case DISPLAY_MESSAGE_STATION:
                lv_label_set_text(ui_Main_Screen_Station, threadMessage.message);
                break;
            case DISPLAY_MESSAGE_COMMERCIAL:
                lv_label_set_text(ui_Main_Screen_Commercial, threadMessage.message);
                lv_obj_remove_flag(uic_Main_Screen_Commercial, LV_OBJ_FLAG_HIDDEN);
                break;
            case DISPLAY_MESSAGE_WIFI_CONNECTED:
                lv_label_set_text(ui_Main_Screen_Station,"");
                lv_obj_remove_flag(ui_Main_Screen_WIFI_Image, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_Main_Screen_No_WIFI_Image, LV_OBJ_FLAG_HIDDEN);

                break;
            case DISPLAY_MESSAGE_WIFI_DISCONNECTED: 
                lv_obj_remove_flag(ui_Main_Screen_No_WIFI_Image, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_Main_Screen_WIFI_Image, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_Main_Screen_Artist, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(uic_Main_Screen_Title, LV_OBJ_FLAG_HIDDEN);
                lv_label_set_text(ui_Main_Screen_Station, "Connection lost");
                break;
            case DISPLAY_MESSAGE_START: 

            break;
            case DISPLAY_MESSAGE_TTIME:
                lv_label_set_text(ui_Main_Screen_Clock_Label, threadMessage.message);
                break;
            case DISPLAY_MESSAGE_ERROR:
                lv_label_set_text(ui_Main_Screen_Station, threadMessage.message);
                break;
        }
    }


    lv_timer_handler();

    if(touchStarted)
    {
       lv_indev_read(indev_touchpad);
    }

}

DisplayTask Display;