#include "DeviceTask.h"
#include <ArduinoLog.h>
#include "OrchestratorTask.h"

#define TS_MINX 300
#define TS_MINY 300
#define TS_MAXX 3800
#define TS_MAXY 3800


#define SS_SWITCH        24

volatile bool DeviceTask::fireRotaryRead = false;
volatile bool DeviceTask::disableInterrupt = false;

DeviceTask::DeviceTask() 
{
  
}

void IRAM_ATTR DeviceTask::rotaryISR()
{
    if(!disableInterrupt)
        fireRotaryRead = true;
}

bool DeviceTask::start()
{
    if(rotary_seesaw.begin(ROTARY_ADDR)){
        Log.infoln("Rotary Seesaw started");

        // use a pin for the built in encoder switch
        // Set the GPIO pin mode for the ESP32 f
        pinMode(18, INPUT_PULLUP);

        // Set the GPIO pin mode for the seesaw push switch
        rotary_seesaw.pinMode(SS_SWITCH, INPUT_PULLUP);

        // Attach the rotary encoder interrupt to pin 18 for ESP32
        attachInterrupt(18, rotaryISR, FALLING);

        // Attach the seesaw switch interrupt
        rotary_seesaw.setGPIOInterrupts((uint32_t)1 << SS_SWITCH, 1);
        // Enable the rotary encoder interrupts
        rotary_seesaw.enableEncoderInterrupt();
    }
    else
    {
        Log.errorln("Rotary Seesaw failed to start");
    }



    return true;
}

bool DeviceTask::begin() 
{
    return Device.start();
}

bool DeviceTask::readRotarySwitch()
{
    return rotary_seesaw.digitalRead(SS_SWITCH);
}

uint32_t DeviceTask::readRotaryPostion()
{
    return rotary_seesaw.getEncoderPosition();
}


void DeviceTask::tick()
{
    if( fireRotaryRead )
    {
        disableInterrupt = true;
 
        Log.infoln("Rotary ISR fired");
       
        if (rotarySwitchBounce == readRotarySwitch()) {
 
        }
        else if(rotarySwitchBounce)
        {
            Log.infoln("Button Released");
            rotarySwitchBounce = false;
        }
        else{
            Log.infoln("Button Pressed");
            rotarySwitchBounce = true;
            Orchestrator.send(ORCHESTRATOR_MESSAGE_ROTARY_PUSH);
        }

        int32_t new_position = readRotaryPostion();
        // did we move around?
        if (encoderPosition != new_position)
        {
            Log.infoln("Rotary position: %d", new_position);

            if(new_position < encoderPosition)
            {
                Orchestrator.send(ORCHESTRATOR_MESSAGE_ROTARY_RIGHT);
                Log.infoln("Rotary turned right");
            }
            else
            {
                Orchestrator.send(ORCHESTRATOR_MESSAGE_ROTARY_LEFT);
                Log.infoln("Rotary turned left");
            }

            encoderPosition = new_position;

        
        }
        
        fireRotaryRead = false;
     
    }
    disableInterrupt = false;
}

DeviceTask Device;