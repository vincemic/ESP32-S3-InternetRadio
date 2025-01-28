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
#include "ConfigurationTask.h"
#include <CloudServiceTask.h>
#include "AsyncFlow.h"


//#define RADIO_STREAM "http://legacy.scahw.com.au/2classicrock_32"
//#define RADIO_STREAM "http://stream.srg-ssr.ch/m/rsp/mp3_128"
//#define RADIO_STREAM "http://www.radioeins.de/frankfurt/livemp3"
//#define RADIO_STREAM "http://vis.media-ice.musicradio.com/CapitalMP3"
//#define RADIO_STREAM "http://rfcm.streamguys1.com/thirdrock-icy"
#define RADIO_STREAM "http://stream.laut.fm/oldies"



bool initialized = false;
uint32_t lastMillis = millis();
AsyncFlow asyncFlow;
String timezone = "";
String ipAddress = "";


// Obtain a list of partitions for this device.
bool logPartitions() {
  const esp_partition_t *partition;
  esp_partition_iterator_t iterator = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);

  if (iterator == nullptr) {
    Log.infoln(F("No partitions found."));
    return false;
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

  return true;
}

bool logMemory() {
    Log.infoln("Total heap: %d", ESP.getHeapSize());
    Log.infoln("Free heap: %d", ESP.getFreeHeap());
    Log.infoln("Total PSRAM: %d", ESP.getPsramSize());
    Log.infoln("Free PSRAM: %d", ESP.getFreePsram());

    return true;
}

bool showMainScreen() {
    return Display.send(DISPLAY_MESSAGE_START, NULL);

}

bool startRadio() {
    return Sound.send(SOUND_MESSAGE_CONNECT, RADIO_STREAM);
}

bool getIPAddress() {
    return CloudService.getIPAddress(ipAddress);
}

bool getTimezone() {
    return CloudService.getTimezone(ipAddress, timezone);
}

bool beginTime() {
    return Time.begin(timezone);
}

void createFlow(){

    asyncFlow.begin([]() -> bool {
        Display.tick();
        return true;
    }, [](const char* label) -> bool {
        Log.infoln("Failure at step %s", label);
        return true;
    });

    asyncFlow.addStep("showscreen", 5000, showMainScreen);
    asyncFlow.addStep("wifi", 500, Wireless.begin);
    asyncFlow.addStep("waitwifi", 1000, Wireless.isWifiConnected, 5);
    asyncFlow.addStep("devices", 40, Device.begin);
    asyncFlow.addStep("sound", 40, Sound.begin);
    asyncFlow.addStep("ip", 5000, getIPAddress);
    asyncFlow.addStep("timezone", 5000, getTimezone);
    asyncFlow.addStep("time", 5000, beginTime);
    asyncFlow.addStep("cloud", 40, CloudService.begin);
    asyncFlow.addStep("partitions", 40, logPartitions);
    asyncFlow.addStep("memory", 40, logMemory);
    asyncFlow.addStep("radio", 40, startRadio);


 
}


void setup()
{

    // Setup foundational components
    
    Serial.begin(115200);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial, false);

    if (!SD.begin(SS)) {
        Log.errorln("error mounting microSD");
    } else {
        Log.infoln("microSD mounted successfully");
    }

    Configuration.begin();
    Display.begin();

    createFlow();
}


void loop() 
{
    if(!initialized) {
      initialized = !asyncFlow.tick();
      return;        
    }
 
    Device.tick();
    Sound.tick();
    Time.tick();
    CloudService.tick();

    // Display last to update all changes
    Display.tick();

    vTaskDelay(10);
}


