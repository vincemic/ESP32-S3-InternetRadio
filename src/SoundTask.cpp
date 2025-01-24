#include <Arduino.h>
#include "SoundTask.h"
#include "DisplayTask.h"
#include <ArduinoLog.h>




SoundTask::SoundTask() 
{
}

bool SoundTask::init()
{
    audio.setPinout(I2S_BCLK, I2S_LRCLK, I2S_DOUT);
    audio.setVolume(2); // 0...21
    audio.setConnectionTimeout(1200,0);  // needed for some stations esp. from around the globe

    xTaskCreatePinnedToCore(
        [](void *parameters)
        {
            Audio audio = *(Audio *)parameters;

            while(true)
            {
                Sound.audio.loop();
                if (!Sound.audio.isRunning())
                {
                
                }
                vTaskDelay(40);
            }
        }
      ,             /* Function to implement the task */
        "audioTask",           /* Name of the task */
        10000,                  /* Stack size in words */
        &audio,                  /* Task input parameter */
        2 | portPRIVILEGE_BIT, /* Priority of the task */
        NULL,                  /* Task handle. */
        0                      /* Core where the task should run */
    );

    return true;
}

void SoundTask::setVolume(uint8_t vol)
{
    audio.setVolume(vol);

}

uint8_t SoundTask::getVolume()
{
    return audio.getVolume();
}

bool SoundTask::connecttohost(const char* host)
{
    return audio.connecttohost(host);
}

bool SoundTask::connecttoSD(const char* filename){
    return true;
}

void SoundTask::turnUpVolume()
{
    uint8_t vol = audio.getVolume();
    if(vol < 21)
    {
        vol++;
        audio.setVolume(vol);
    }
}

void SoundTask::turnDownVolume()
{
    uint8_t vol = audio.getVolume();
    if(vol > 0)
    {
        vol--;
        audio.setVolume(vol);
    }
}

void SoundTask::tick()
{
    ThreadMessage message;
    if(internalReceive(&message))
    {
        switch (message.messageType)
        {
            case SOUND_MESSAGE_TURN_UP_VOLUME:
                turnUpVolume();
                break;
            case SOUND_MESSAGE_TURN_DOWN_VOLUME:
                turnDownVolume();
                break;
            case SOUND_MESSAGE_CONNECT:
                if(!connecttohost(message.message))
                {
                    Display.send(DISPLAY_MESSAGE_ERROR, "Connection to stream failed");
                }
                break;
            default:
                break;
        }
    }
}
SoundTask Sound;

