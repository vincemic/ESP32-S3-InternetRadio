#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Audio.h>
#include <TFT_eSPI.h>
#include "lvgl.h"
#include "ui/ui.h"
#include "SoundTask.h"
#include "DisplayTask.h"
#include "esp_mac.h"
#include "DeviceTask.h"
#include "WirelessTask.h"
#include <ArduinoLog.h>
#include "TimeTask.h"


//#define RADIO_STREAM "http://legacy.scahw.com.au/2classicrock_32"
//#define RADIO_STREAM "http://stream.srg-ssr.ch/m/rsp/mp3_128"
//#define RADIO_STREAM "http://www.radioeins.de/frankfurt/livemp3"
//#define RADIO_STREAM "http://vis.media-ice.musicradio.com/CapitalMP3"
//#define RADIO_STREAM "http://rfcm.streamguys1.com/thirdrock-icy"
#define RADIO_STREAM "http://stream.laut.fm/oldies"



bool initialized = false;
uint32_t lastMillis = millis();

// Obtain a list of partitions for this device.
void logPartitions() {
  const esp_partition_t *partition;
  esp_partition_iterator_t iterator = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);

  if (iterator == nullptr) {
    Log.infoln(F("No partitions found."));
    return;
  }

  Log.infoln(F("Partitions:"));
  while (iterator != nullptr) {
    partition = esp_partition_get(iterator);
    Log.infoln("Label: %s, Size: %lu bytes, Address: 0x%08lx",
                  partition->label,
                  partition->size,
                  partition->address);
    iterator = esp_partition_next(iterator);
  }

  esp_partition_iterator_release(iterator);  // Release the iterator once done
}

void logMemory() {
    Log.infoln("Total heap: %d", ESP.getHeapSize());
    Log.infoln("Free heap: %d", ESP.getFreeHeap());
    Log.infoln("Total PSRAM: %d", ESP.getPsramSize());
    Log.infoln("Free PSRAM: %d", ESP.getFreePsram());
}

void setup()
{
    Serial.begin(115200);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial, false);

    if (!SD.begin(SS)) {
        Log.errorln("error mounting microSD");
    } else {
        Log.infoln("microSD mounted successfully");
    }

    Display.init();

}

void initialize()
{
    Wireless.init();
    Device.init();
    Sound.init();
    Time.init();

    logPartitions();
    logMemory();



    Display.send(DISPLAY_MESSAGE_START, NULL);
    Sound.send(SOUND_MESSAGE_CONNECT, RADIO_STREAM);

    initialized = true;
}

void loop() 
{

    Display.tick();

    // Check if the system is initialized
    // If not, initialize it
    if(! initialized && millis() - lastMillis > 10000)
    {
        initialize();
    }

    // Check if the system is initialized
    // If so, tick the device and sound tasks
    if(initialized)
    {
        Device.tick();
        Sound.tick();
        Time.tick();
    }

    vTaskDelay(10);
}

