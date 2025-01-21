#include "DisplayTask.h"
#include "SoundTask.h"
#include "lv_tc.h"
#include "lv_tc_screen.h"
#include "esp_nvs_tc.h"

//#define RADIO_STREAM "http://legacy.scahw.com.au/2classicrock_32"
//#define RADIO_STREAM "http://stream.srg-ssr.ch/m/rsp/mp3_128"
//#define RADIO_STREAM "http://www.radioeins.de/frankfurt/livemp3"
//#define RADIO_STREAM "http://vis.media-ice.musicradio.com/CapitalMP3"
//#define RADIO_STREAM "http://rfcm.streamguys1.com/thirdrock-icy"
#define RADIO_STREAM "http://stream.laut.fm/oldies"

static ulong getMills() { return millis(); };

static void drag_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = ( lv_obj_t *) lv_event_get_target(e);

    lv_indev_t * indev = lv_indev_get_act();
    if(indev == NULL)  return;
     
    lv_point_t pos;
    lv_indev_get_point(indev, &pos);

    lv_obj_set_pos(obj, pos.x ,  pos.y );
}

DisplayTask::DisplayTask() {
  
 
}


bool DisplayTask::init()
{

    lv_log_register_print_cb([](lv_log_level_t level, const char * message) {
        Serial.printf("%s\n", message);
    });


    lv_init();

    // Initialize the TFT display using the TFT_eSPI library
    display = lv_tft_espi_create(TFT_WIDTH, TFT_HEIGHT, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(display, LV_DISPLAY_ROTATION_270);

    Serial.printf("Screen horizontal: %d", lv_display_get_horizontal_resolution(display));
    Serial.printf("Screen vertical: %d", lv_display_get_vertical_resolution(display));

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(getMills);


    if(touchController.begin(TOUCH_ADDR)){
        Serial.println("Touch controller started");
    }
    else
    {
        Serial.println("Touch controller failed to start");
    }

    /*Register a touchpad input device*/
    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, readTouchCB);

    ui_init();
  
    lv_obj_add_event_cb(ui_Screen1_Image4, drag_event_handler, LV_EVENT_PRESSING, NULL);
       
    lv_timer_handler();
    
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
        Serial.print("Touch point: (");
        Serial.print(x); Serial.print(", ");
        Serial.print(y); 
        Serial.println(")");

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

        Serial.print("Result point: (");
        Serial.print(x_diff); Serial.print(", ");
        Serial.print(y_diff); 
        Serial.println(")");


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
    if(Sound.internalReceive(&threadMessage)) 
    {
        switch(threadMessage.messageType)
        {
            case DISPLAY_MESSAGE_ARTIST:
                lv_label_set_text(ui_Screen1_Artist, threadMessage.message);
                break;
            case DISPLAY_MESSAGE_TITLE:
                lv_label_set_text(ui_Screen1_Title, threadMessage.message);
                break;
            case DISPLAY_MESSAGE_STATION:
                lv_label_set_text(ui_Screen1_Station, threadMessage.message);
                break;
        }
        
    }
    lv_timer_handler();
    lv_indev_read(Display.indev_touchpad);
}

void onButtonClicked(lv_event_t * e)
{

    Sound.connecttohost(RADIO_STREAM);
 
}



DisplayTask Display;