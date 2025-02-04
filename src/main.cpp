#include <Arduino.h>
#include <ArduinoLog.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "OrchestratorTask.h"
#include <ConfigurationTask.h>
#include <DisplayTask.h>

SPIClass *spi_onboardSD = new SPIClass(FSPI);

void setup()
{
    Serial.begin(115200);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial, false);

    spi_onboardSD->begin();

    if (!SD.begin(SS, *spi_onboardSD)) {
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


