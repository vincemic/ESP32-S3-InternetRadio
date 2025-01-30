#include <Arduino.h>
#include <ArduinoLog.h>
#include "OrchestratorTask.h"

void setup()
{

    Serial.begin(115200);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial, false);

    if (!SD.begin(SS)) {
        Log.errorln("error mounting microSD");
    } else {
        Log.infoln("microSD mounted successfully");
    }

    Orchestrator.begin();
    Configuration.begin();
    Display.begin();

    Orchestrator.send(ORCHESTRATOR_START);
}


void loop() 
{
    Orchestrator.tick();
    vTaskDelay(10);
}


