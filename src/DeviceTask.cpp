#include "DeviceTask.h"

#define TS_MINX 300
#define TS_MINY 300
#define TS_MAXX 3800
#define TS_MAXY 3800

DeviceTask Device;

DeviceTask::DeviceTask() 
{
    
}

bool DeviceTask::init()
{
    sspixel = seesaw_NeoPixel(1, 6, NEO_GRB + NEO_KHZ800);


    if(rotary_seesaw.begin(ROTARY_ADDR)){
        Serial.println("Rotary Seesaw started");
    }
    else
    {
        Serial.println("Rotary Seesaw failed to start");
    }

    if(touch.begin(TOUCH_ADDR)){
        Serial.println("Touch Seesaw started");
    }
    else
    {
        Serial.println("Touch Seesaw failed to start");
    }
    
    sspixel.begin(SEESAW_ADDRESS);

     // set not so bright!
    sspixel.setBrightness(20);
    sspixel.show();
    
    // use a pin for the built in encoder switch
    rotary_seesaw.pinMode(SS_SWITCH, INPUT_PULLUP);

    // get starting position
    encoder_position = rotary_seesaw.getEncoderPosition();

    Serial.printf("Starting encoder position: %d\n", encoder_position);

    /*Register a touchpad input device*/
    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, readTouchCB);
    lv_indev_set_driver_data(indev_touchpad, this);

    return true;
}

uint32_t DeviceTask::readRotarySwitch()
{
    return rotary_seesaw.digitalRead(SS_SWITCH);
}

uint32_t DeviceTask::readRotaryPostion()
{
    return rotary_seesaw.getEncoderPosition();
}

uint32_t DeviceTask::color(uint8_t r, uint8_t g, uint8_t b)
{
    return sspixel.Color(r, g,b);
 
}

void DeviceTask::setRotaryPixelColor(uint16_t n, uint32_t c)
{
    sspixel.setPixelColor(n, c);
    sspixel.show();
 
}

void DeviceTask::readTouchCB(lv_indev_t *device, lv_indev_data_t *data)
{
    uint16_t x,y,z1,z2;
    int16_t x_diff, y_diff;
    data->continue_reading = true;

    Serial.println("Touch CB");

    if(Device.readTouch(&x, &y, &z1, &z2)){
        if(z1 > 100) {
            x_diff = x;
            y_diff = y;
            Serial.print("Touch point: (");
            Serial.print(x_diff); Serial.print(", ");
            Serial.print(y_diff); Serial.print(", ");
            Serial.print(z1); Serial.print(" / ");
            Serial.print(z2); Serial.println(")");
            //data->point.x = map(x, TS_MINX, TS_MAXX, 0, SCREEN_WIDTH);
            //data->point.y = map(y, TS_MINY, TS_MAXY, 0, SCREEN_HEIGHT);


            data->state = LV_INDEV_STATE_PRESSED;
        } else {
            data->point.x =0;
            data->point.y = 0;
            data->state = LV_INDEV_STATE_RELEASED;
            Serial.println("Touch released");
        }
    }

}

bool DeviceTask::readTouch(uint16_t* x,uint16_t* y,uint16_t* z1,uint16_t* z2)
{
    return touch.read_touch(x,y,z1,z2);
}