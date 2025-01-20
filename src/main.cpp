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

//#define RADIO_STREAM "http://legacy.scahw.com.au/2classicrock_32"
//#define RADIO_STREAM "http://stream.srg-ssr.ch/m/rsp/mp3_128"
//#define RADIO_STREAM "http://www.radioeins.de/frankfurt/livemp3"
//#define RADIO_STREAM "http://vis.media-ice.musicradio.com/CapitalMP3"
//#define RADIO_STREAM "http://rfcm.streamguys1.com/thirdrock-icy"
#define RADIO_STREAM "http://stream.laut.fm/oldies"



volatile uint32_t song_count = 0; 
const char ssid[] = "missile";
const char pass[] = "vincemic123!"; 

uint32_t rotary_position = 0;

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

    Sound.connecttohost(RADIO_STREAM);


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
     if (! Device.readRotarySwitch()) {
        Serial.println("Button pressed!");
    }

    int32_t new_position = Device.readRotaryPostion();
    // did we move arounde?
    if (rotary_position != new_position)
    {
        Serial.println(new_position);         // display new position
        rotary_position = new_position;      // and save for next round
    }

   Display.tick();
   delay(10);  
}



