#include "DisplayTask.h"
#include "SoundTask.h"
#include <ArduinoLog.h>
#include "ConfigurationTask.h"
#include <OrchestratorTask.h>

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

    // populate the wifi credentials
    lv_textarea_set_text(ui_Network_Screen_SSID_Text_Area, Configuration.getWifiSSID().c_str());
    lv_textarea_set_text(uic_Network_Screen_Password_Text_Area, Configuration.getWifiPassword().c_str());

    // blank labels to start
    lv_label_set_text(ui_Main_Screen_Artist, "");
    lv_label_set_text(ui_Main_Screen_Title, "");
    lv_label_set_text(ui_Main_Screen_Station, "");
    lv_label_set_text(ui_Main_Screen_Commercial, "");
    lv_label_set_text(uic_Message_Screen_Message_Label, "");
    lv_label_set_text(ui_Main_Screen_Clock_Label, "");
   
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
                break;
            case DISPLAY_MESSAGE_COMMERCIAL:
                lv_label_set_text(ui_Main_Screen_Commercial, threadMessage.message);
                break;
            case DISPLAY_MESSAGE_WIFI_CONNECTED:
                lv_obj_remove_flag(ui_Main_Screen_WIFI_Image, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_Main_Screen_No_WIFI_Image, LV_OBJ_FLAG_HIDDEN);
                break;
            case DISPLAY_MESSAGE_WIFI_DISCONNECTED: 
                lv_obj_remove_flag(ui_Main_Screen_No_WIFI_Image, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui_Main_Screen_WIFI_Image, LV_OBJ_FLAG_HIDDEN);
                break;
            case DISPLAY_MESSAGE_ERROR:
                break;
            case DISPLAY_MESSAGE_SCREEN_MESSAGE_MESSAGE:
                lv_label_set_text(uic_Message_Screen_Message_Label, threadMessage.message);
                break;    
            case DISPLAY_MESSAGE_TUNE_UP:

                number = lv_roller_get_option_count(uic_Station_Selection_Screen_Roller);
                selected = lv_roller_get_selected(uic_Station_Selection_Screen_Roller);

                if(selected < number - 1) 
                    lv_roller_set_selected (uic_Station_Selection_Screen_Roller,++selected, LV_ANIM_OFF);

                Orchestrator.send(ORCHESTRATOR_MESSAGE_STATION_SELECTED);
                break;

            case DISPLAY_MESSAGE_TUNE_DOWN:

                number = lv_roller_get_option_count(uic_Station_Selection_Screen_Roller);
                selected = lv_roller_get_selected(uic_Station_Selection_Screen_Roller);

                if(selected > 0) 
                    lv_roller_set_selected(uic_Station_Selection_Screen_Roller,--selected, LV_ANIM_OFF);

                Orchestrator.send(ORCHESTRATOR_MESSAGE_STATION_SELECTED);
                break;
            case DISPLAY_MESSAGE_UPDATE_CLOCK:
                lv_label_set_text(uic_Clock_Screen_Clock_Label, threadMessage.message);
                lv_label_set_text(ui_Main_Screen_Clock_Label, threadMessage.message);
                break;

            case DISPLAY_MESSAGE_UPDATE_STATIONS:
                Log.infoln("Updating stations selection UI");
                updateStationListDisplay();
                break;


            case DISPLAY_MESSAGE_SCREEN_RADIO:
            case DISPLAY_MESSAGE_SCREEN_CLOCK:
            case DISPLAY_MESSAGE_SCREEN_STATION_SELECTION:
            case DISPLAY_MESSAGE_SCREEN_NETWORK:
            case DISPLAY_MESSAGE_SCREEN_MODE:
            case DISPLAY_MESSAGE_SCREEN_LOADING:
            case DISPLAY_MESSAGE_SCREEN_MESSAGE:
                showScreen(threadMessage.messageType);
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

String DisplayTask::getSelectedStation()
{
    char stationName[100];
    lv_roller_get_selected_str(uic_Station_Selection_Screen_Roller, stationName,100);
    return String(stationName);
}

void DisplayTask::showScreen(uint16_t screenId)
{
    switch(screenId)
    {
        case DISPLAY_MESSAGE_SCREEN_RADIO:
                lv_screen_load(ui_Main_Screen);
        break;
        case DISPLAY_MESSAGE_SCREEN_CLOCK:  
                lv_screen_load(uic_Clock_Screen);
        break;
        case DISPLAY_MESSAGE_SCREEN_STATION_SELECTION:          
                lv_screen_load(ui_Station_Selection_Screen);
        break;
        case DISPLAY_MESSAGE_SCREEN_NETWORK:
                lv_screen_load(uic_Network_Screen);
        break;
        case DISPLAY_MESSAGE_SCREEN_MODE:
                lv_screen_load(uic_Mode_Screen);
        break;
        case DISPLAY_MESSAGE_SCREEN_LOADING:    
                lv_screen_load(uic_Loading_Screen);
        break;
        case DISPLAY_MESSAGE_SCREEN_MESSAGE:    
                lv_screen_load(uic_Message_Screen);
                lv_label_set_text(uic_Message_Screen_Message_Label, "");
        break;
    }

}

void DisplayTask::updateStationListDisplay()
{

    /*
    // try and keep selection in the second (middle) set of 3 stations sets
   uint32_t selectedIndex =  lv_roller_get_selected(uic_Station_Selection_Screen_Roller);

    // is the selected index in the third set of stations, then load a new page of station sets
    if(selectedIndex > STATION_SET_SIZE * 2)
    {
        lv_roller_set_selected(uic_Station_Selection_Screen_Roller, selectedIndex - 1, LV_ANIM_OFF);

    } else if(selectedIndex < STATION_SET_SIZE) { // is the selected index in the first set of stations, then load a new page of station sets

        lv_roller_set_selected(uic_Station_Selection_Screen_Roller, selectedIndex + 1, LV_ANIM_OFF);
    }
    */

    lv_roller_set_options(uic_Station_Selection_Screen_Roller, createStaionListPage(160), LV_ROLLER_MODE_NORMAL); 


}

const char * DisplayTask::createStaionListPage(size_t stationIndex)
{
    size_t pageStartIndex = 0;
    size_t pageEndIndex = 0;

    // No data
    if(Orchestrator.stationSetCount == 0)
    {
        Log.errorln("No station data");
        return "";
    }


    // get the station set index that contains the station index
    size_t stationSetIndex = floor(stationIndex / STATION_SET_SIZE);

    // check for bad stationIndex value which yielded a
    // bad stationSetIndex value
    if(stationSetIndex >= Orchestrator.stationSetCount) {
        Log.errorln("Bad Station index: %d, station set index: %d", stationIndex, stationSetIndex);
        return "";

    }
       
     
    if(stationSetIndex == 0)
    {
        // if the station index is in the first set of stations
        // then load the first three stations sets
        pageStartIndex = 0;
        pageEndIndex = Orchestrator.stationSetIndexes[stationSetIndex + 3] - 1;

    }
    else 
    {
        // if the station index is in the last set of stations
        // then load the last three stations sets
        if(stationSetIndex == Orchestrator.stationSetCount - 1)
        {
            Log.infoln("Station index is in the last set of stations");
            pageStartIndex = Orchestrator.stationSetIndexes[stationSetIndex - 2];
            pageEndIndex = Orchestrator.stationSetIndexes[stationSetIndex + 1] - 1;
        }
        else
        {
            Log.infoln("Station index is in the middle set of stations");
            // if the station index is in the middle set of stations
            // then load the middle three stations sets
            pageStartIndex = Orchestrator.stationSetIndexes[stationSetIndex - 1];
            pageEndIndex = Orchestrator.stationSetIndexes[stationSetIndex + 2] - 1;
        }
    }
    char value = Orchestrator.stationNamesString[pageEndIndex];
    Orchestrator.stationNamesString[pageEndIndex] = '\0';
    Log.infoln("Station index: %d, station set index: %d, page start index: %d, page end index: %d", stationIndex, stationSetIndex, pageStartIndex, pageEndIndex);
    Log.infoln("Station set count: %d", Orchestrator.stationSetCount);
    Log.infoln("Station set value:\n %s", Orchestrator.stationNamesString + pageStartIndex);
   // Orchestrator.stationNamesString[pageEndIndex] = value;

    return Orchestrator.stationNamesString + pageStartIndex;
}

DisplayTask Display;