#include <Arduino.h>
#include "SoundTask.h"
#include "DisplayTask.h"
#include <ArduinoLog.h>
#include "ConfigurationTask.h"
#include "ui/ui.h"



SoundTask::SoundTask() 
{
}

bool SoundTask::start()
{
    audio.setPinout(I2S_BCLK, I2S_LRCLK, I2S_DOUT);
    audio.setConnectionTimeout(1200,0);  // needed for some stations esp. from around the globe

    auto volume = Configuration.getLastVolume();

    if(volume > audio.maxVolume())
    {
        volume = audio.maxVolume();
    }

    audio.setVolume(volume);

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

bool SoundTask::begin()
{
   return Sound.start();

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
    if(vol < audio.maxVolume())
    {
        vol++;
        audio.setVolume(vol);
        Configuration.setLastVolume(vol);
    }
}

void SoundTask::turnDownVolume()
{
    uint8_t vol = audio.getVolume();
    if(vol > 0)
    {
        vol--;
        audio.setVolume(vol);
        Configuration.setLastVolume(vol);
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

