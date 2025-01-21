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

void setup()
{
    Serial.begin(115200);
    delay(10000);

    if (!SD.begin(SS)) {
        Serial.println("error mounting microSD");
    } else {
        Serial.println("microSD mounted successfully");
    }

    Serial.println("running example \"Play-Radio-Station-with-TFT\":");

    // connecting to local WiFi network
    Serial.printf("connecting to WiFi network \"%s\"\n", ssid);
    WiFi.begin(ssid, pass);
    while ( WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(2000);
    }
    Serial.printf("\n connected successfully to \"%s\". IP address: %s\n", ssid, WiFi.localIP().toString());

    Device.init();
    Display.init();

    Sound.init();

    Sound.setVolume(1);

    Serial.printf("Total heap: %d\n", ESP.getHeapSize());
    Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
    Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
    Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());

  


}


uint32_t wheel(uint32_t wheelPos) {
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85) {
    return Device.color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if (wheelPos < 170) {
    wheelPos -= 85;
    return Device.color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return Device.color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

void loop() 
{

  Device.tick();
  Display.tick();
  vTaskDelay(100);
}



