#include "DisplayTask.h"
#include "SoundTask.h"
#include <ArduinoLog.h>
#include "ConfigurationTask.h"

//#define RADIO_STREAM "http://legacy.scahw.com.au/2classicrock_32"
//#define RADIO_STREAM "http://stream.srg-ssr.ch/m/rsp/mp3_128"
//#define RADIO_STREAM "http://www.radioeins.de/frankfurt/livemp3"
//#define RADIO_STREAM "http://vis.media-ice.musicradio.com/CapitalMP3"
//#define RADIO_STREAM "http://rfcm.streamguys1.com/thirdrock-icy"
#define RADIO_STREAM "http://stream.laut.fm/oldies"

static uint32_t  getMills() { return esp_timer_get_time() / 1000; };

volatile bool DisplayTask::fireTouchRead = false;
volatile bool DisplayTask::disableInterrupt = false;

void IRAM_ATTR DisplayTask::touchISR()
{
    if(!disableInterrupt)
        fireTouchRead = true;
}

DisplayTask::DisplayTask() {
  
 
}

bool DisplayTask::start()
{
    // use a pin for touch screen interrupt
    // Set the GPIO pin mode for the ESP32 f
    pinMode(17, INPUT_PULLUP);

    // Attach the interrupt to pin 17 for ESP32
    attachInterrupt(17, touchISR, FALLING);

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


    // Disable the polling timer all together for interrupts
    lv_timer_del(indev_touchpad->read_timer);

    ui_init();

    lv_obj_add_flag(ui_Main_Screen_Clock_Label, LV_OBJ_FLAG_HIDDEN);

    showMessageUI("Starting...");

    // populate the wifi credentials
    lv_textarea_set_text(ui_Network_Screen_SSID_Text_Area, Configuration.getWifiSSID().c_str());
    lv_textarea_set_text(uic_Network_Screen_Password_Text_Area, Configuration.getWifiPassword().c_str());

    while(!touchController.begin(TOUCH_ADDR))
    { 
         delay(1000);
    }


    touchStarted = true;
    Log.infoln("Touch controller started");
       
    return true;
}
   
bool DisplayTask::begin()
{
    return Display.start();
}

void DisplayTask::readTouchCB(lv_indev_t *device, lv_indev_data_t *data)
{
    static uint16_t highX = 0;
    static uint16_t highY = 0;

    uint16_t x,y,z1,z2;
    int32_t x_diff, y_diff;

    if(Display.readTouch(&x, &y, &z1, &z2)&& z1 > 10)
    {
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

        Log.infoln("Touch point: (%d, %d)", x_diff, y_diff);

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

void DisplayTask::showRadioUI()
{

    lv_obj_clear_flag(ui_Main_Screen_Artist, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(uic_Main_Screen_Title, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(uic_Main_Screen_Commercial, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(uic_Main_Screen_Station, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(uic_Main_Screen_Mode_Button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(uic_Main_Screen_Tune_Button, LV_OBJ_FLAG_HIDDEN);
    
    lv_obj_add_flag(ui_Main_Screen_Message_Label, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui_Main_Screen_Message_Label, "");
}

void DisplayTask::showMessageUI(const char * message)
{
    lv_obj_add_flag(uic_Main_Screen_Station, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Main_Screen_Artist, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(uic_Main_Screen_Title, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(uic_Main_Screen_Commercial, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(uic_Main_Screen_Mode_Button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(uic_Main_Screen_Tune_Button, LV_OBJ_FLAG_HIDDEN);


    lv_label_set_text(ui_Main_Screen_Artist,"");
    lv_label_set_text(uic_Main_Screen_Title, "");
    lv_label_set_text(uic_Main_Screen_Commercial, "");
    lv_label_set_text(uic_Main_Screen_Station, "");

    lv_obj_clear_flag(ui_Main_Screen_Message_Label, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text(ui_Main_Screen_Message_Label, message);

}

void DisplayTask::tick()
{
    static uint32_t lastTime = 0;
    ThreadMessage threadMessage;  
    static uint16_t stationListIndex = 0;
    uint32_t selected = 0;
    uint32_t number = 0;
    char stationName[200];

    if(internalReceive(&threadMessage)) 
    {
        Log.infoln("Display message %d received: %s ", threadMessage.messageType, threadMessage.message);
        switch(threadMessage.messageType)
        {
            case DISPLAY_MESSAGE_ARTIST:
                lv_label_set_text(ui_Main_Screen_Artist, threadMessage.message);
                break;
            case DISPLAY_MESSAGE_TITLE:
                lv_label_set_text(ui_Main_Screen_Title, threadMessage.message);
                break;
            case DISPLAY_MESSAGE_STATION:
                lv_label_set_text(ui_Main_Screen_Station, threadMessage.message);
                showRadioUI();
                break;
            case DISPLAY_MESSAGE_COMMERCIAL:
                lv_label_set_text(ui_Main_Screen_Commercial, threadMessage.message);
                break;
            case DISPLAY_MESSAGE_WIFI_CONNECTED:

                lv_obj_remove_flag(ui_Main_Screen_WIFI_Image, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_Main_Screen_No_WIFI_Image, LV_OBJ_FLAG_HIDDEN);

                showMessageUI(threadMessage.message);

                break;
            case DISPLAY_MESSAGE_WIFI_DISCONNECTED: 
                // WiFi Icons
                lv_obj_remove_flag(ui_Main_Screen_No_WIFI_Image, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_Main_Screen_WIFI_Image, LV_OBJ_FLAG_HIDDEN);
                
                showMessageUI(threadMessage.message);

                break;
            case DISPLAY_MESSAGE_START: 
                lv_screen_load(ui_Main_Screen);

                showMessageUI(threadMessage.message);

                break;
            case DISPLAY_MESSAGE_TTIME:
                // show time label is hidden
                lv_obj_remove_flag(ui_Main_Screen_Clock_Label, LV_OBJ_FLAG_HIDDEN);
                //Main screen header
                lv_label_set_text(ui_Main_Screen_Clock_Label, threadMessage.message);
                // Clock screen
                lv_label_set_text(uic_Clock_Screen_Clock_Label, threadMessage.message);
                break;
            case DISPLAY_MESSAGE_ERROR:
                showMessageUI(threadMessage.message);
                break;


            case DISPLAY_MESSAGE_WIFI_CONNECTING:
                // Wifi Icons
                lv_obj_add_flag(ui_Main_Screen_WIFI_Image, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui_Main_Screen_No_WIFI_Image, LV_OBJ_FLAG_HIDDEN);

                showMessageUI(threadMessage.message);

                break; 

            case DISPLAY_MESSAGE_TUNE_UP:

                if(Display.getActiveScreen() != ui_Station_Selection_Screen)
                {
                    break;
                }

                number = lv_roller_get_option_count(uic_Station_Selection_Screen_Roller);
                selected = lv_roller_get_selected(uic_Station_Selection_Screen_Roller);

                if(selected < number - 1) {
                    lv_roller_set_selected (uic_Station_Selection_Screen_Roller,++selected, LV_ANIM_OFF);
                    Log.infoln("Tune up");
                }
                break;

            case DISPLAY_MESSAGE_TUNE_DOWN:

                if(Display.getActiveScreen() != ui_Station_Selection_Screen)
                {
                    break;
                }

                selected = lv_roller_get_selected(uic_Station_Selection_Screen_Roller);

                if(selected > 0) {
                    lv_roller_set_selected(uic_Station_Selection_Screen_Roller,--selected, LV_ANIM_OFF);
                    Log.infoln("Tune down");
                }
                break;
            case DISPLAY_MESSAGE_SELECT:

                if(Display.getActiveScreen() == ui_Station_Selection_Screen)
                {
                    lv_roller_get_selected_str(uic_Station_Selection_Screen_Roller,stationName,200);
                    lv_screen_load(ui_Main_Screen);
                    Log.infoln("Selected station: %s", stationName);
                }
                
                break;
            case DISPLAY_MESSAGE_TUNING_SCREEN:
                lv_screen_load(uic_Tune_Screen);
            break;

            case DISPLAY_MESSAGE_STATION_LIST:   
            case DISPLAY_MESSAGE_FIND_IPADDRESS:
            case DISPLAY_MESSAGE_FIND_TIMEZONE:
            case DISPLAY_MESSAGE_IPADDRESS:
            case DISPLAY_MESSAGE_TIMEZONE:
                lv_label_set_text(ui_Main_Screen_Message_Label, threadMessage.message);
                break;
        }
    }

    // If we get an interrupt from the touch controller, then keep reading for a few more cycles
    // this prevents drop outs on the touch screen
    static uint16_t touchRunout = 201;
    if(fireTouchRead || touchRunout++ < 200) 
    {
        // reset the runout timer if we got another interrupt
       if(fireTouchRead)    
            touchRunout = 0;

        disableInterrupt = true;
        lv_indev_read(Display.indev_touchpad);
        fireTouchRead = false;
       
    } 

    disableInterrupt = false;
    lv_timer_handler();

}

lv_obj_t * DisplayTask::getActiveScreen()
{
    return lv_screen_active();
}

DisplayTask Display;