#include "DisplayTask.h"
#include "SoundTask.h"

DisplayTask::DisplayTask() {
  
 
}

bool DisplayTask::init()
{

    xTaskCreatePinnedToCore(
        [](void *parameters)
        {
            DisplayTask *displayTaskp = (DisplayTask*) parameters;
            while(true)
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

                vTaskDelay(100); 
            }
        }
        ,             /* Function to implement the task */
        "displayTask",           /* Name of the task */
        10000,                  /* Stack size in words */
        this,                  /* Task input parameter */
        2 | portPRIVILEGE_BIT, /* Priority of the task */
        NULL,                  /* Task handle. */
        1                      /* Core where the task should run */
    );

    return true;
}


DisplayTask Display;