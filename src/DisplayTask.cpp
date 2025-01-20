#include "DisplayTask.h"
#include "SoundTask.h"

static ulong getMills() { return millis(); };

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

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(getMills);

    ui_init();

    /*Register a touchpad input device*/
    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, readTouchCB);
    lv_indev_set_mode(indev_touchpad, LV_INDEV_MODE_TIMER);

    if(touchController.begin(TOUCH_ADDR)){
        Serial.println("Touch controller started");
    }
    else
    {
        Serial.println("Touch controller failed to start");
    }

    lv_timer_handler();
    
     return true;
}
   
void DisplayTask::readTouchCB(lv_indev_t *device, lv_indev_data_t *data)
{
    uint16_t x,y,z1,z2;
    int32_t x_diff = 0, y_diff = 0;

    if(Display.readTouch(&x, &y, &z1, &z2) && z1 > 6)
    {
        x_diff = map(x, 0, 5000, 0, TFT_HEIGHT);
        y_diff = map(y, 0, 5000, 0,  TFT_WIDTH);

        if(x_diff < 0) 
            x_diff = 0;
        if(y_diff < 0) 
            y_diff = 0;

        if(x_diff > TFT_HEIGHT)
             x_diff = TFT_HEIGHT;
        if(y_diff > TFT_WIDTH)
             y_diff = TFT_WIDTH;

        Serial.print(x); 
        Serial.print(" - ");
        Serial.println(y);   
        Serial.print("Touch point: (");
        Serial.print(x_diff); Serial.print(", ");
        Serial.print(y_diff); Serial.print(", ");
        Serial.print(z1); Serial.print(" / ");
        Serial.print(z2); Serial.println(")");


        data->point.x = x_diff;
        data->point.y = y_diff;

        if(z1 > 100)
            data->state = LV_INDEV_STATE_PRESSED;
        else
            data->state = LV_INDEV_STATE_RELEASED;

    } else {   

        data->point.x = 0;
        data->point.y =0;
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

DisplayTask Display;