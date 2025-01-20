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
    //sspixel = seesaw_NeoPixel(1, 6, NEO_GRB + NEO_KHZ800);


    if(rotary_seesaw.begin(ROTARY_ADDR)){
        Serial.println("Rotary Seesaw started");
    }
    else
    {
        Serial.println("Rotary Seesaw failed to start");
    }

    
    //sspixel.begin(SEESAW_ADDRESS);

     // set not so bright!
    //sspixel.setBrightness(20);
    //sspixel.show();
    
    // use a pin for the built in encoder switch
    rotary_seesaw.pinMode(SS_SWITCH, INPUT_PULLUP);

    // get starting position
    encoder_position = rotary_seesaw.getEncoderPosition();

    Serial.printf("Starting encoder position: %d\n", encoder_position);



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

