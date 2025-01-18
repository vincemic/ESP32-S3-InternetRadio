#include "DeviceTask.h"


DeviceTask Device;

DeviceTask::DeviceTask() 
{
    
}

bool DeviceTask::init()
{
    /*
    if(Wire.begin(8, 9))
    {
        Serial.println("Wire started");
    }
    else
    {
        Serial.println("Wire failed to start");
    }

    if(rotary_seesaw.begin(ROTARY_SEESAW_ADDR)){
        Serial.println("Rotary Seesaw started");
    }
    else
    {
        Serial.println("Rotary Seesaw failed to start");
    }
    */

    return true;
}
