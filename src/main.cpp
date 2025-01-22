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
#include "ThreadTask.h"
#include "DeviceTask.h"




const char ssid[] = "missile";
const char pass[] = "vincemic123!"; 

bool initialized = false;
uint32_t lastMillis = millis();

void setup()
{
    Serial.begin(115200);

    if (!SD.begin(SS)) {
        Serial.println("error mounting microSD");
    } else {
        Serial.println("microSD mounted successfully");
    }

    Display.init();

}

void initialize()
{
    // connecting to local WiFi network
    Serial.printf("connecting to WiFi network \"%s\"\n", ssid);
    WiFi.begin(ssid, pass);
    while ( WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(2000);
    }

    Display.send(DISPLAY_MESSAGE_WIFI_CONNECTED, "WiFi connected");

    Serial.printf("\n connected successfully to \"%s\". IP address: %s\n", ssid, WiFi.localIP().toString());

    Device.init();
    Sound.init();

    Serial.printf("Total heap: %d\n", ESP.getHeapSize());
    Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
    Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());

    initialized = true;
}


void scheduler()
{
    if(! initialized && millis() - lastMillis > 30000)
    {
        initialize();
    }
}

void loop() 
{

  Display.tick();

  scheduler();

  if(initialized)
  {
     Device.tick();
  }

  vTaskDelay(100);
}
