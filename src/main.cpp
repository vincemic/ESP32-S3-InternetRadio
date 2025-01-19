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
//#include <esp_mac.h>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

//#define RADIO_STREAM "http://legacy.scahw.com.au/2classicrock_32"
//#define RADIO_STREAM "http://stream.srg-ssr.ch/m/rsp/mp3_128"
//#define RADIO_STREAM "http://www.radioeins.de/frankfurt/livemp3"
//#define RADIO_STREAM "http://vis.media-ice.musicradio.com/CapitalMP3"
//#define RADIO_STREAM "http://rfcm.streamguys1.com/thirdrock-icy"
#define RADIO_STREAM "http://stream.laut.fm/oldies"

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];
lv_display_t * disp;
volatile uint32_t song_count = 0; 
const char ssid[] = "missile";
const char pass[] = "vincemic123!"; 

static ulong getMills() { return (esp_timer_get_time() / 1000LL); };

uint32_t rotary_position = 0;

void setup()
{
    Serial.begin(115200);
    delay(10000);

    // FSPI default definitions for SS, MOSI, SCK & MISO are 10, 11, 12 & 13 
    // (see pins_arduino.h for more info) and match the boards wiring
    //spi_onboardSD->begin();

    if (!SD.begin(SS)) {
        Serial.println("error mounting microSD");
    } else {
        Serial.println("microSD mounted successfully");
    }

    lv_init();

    // Initialize the TFT display using the TFT_eSPI library
    disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(getMills);
    
    ui_init();

    Serial.println("running example \"Play-Radio-Station-with-TFT\":");

    // connecting to local WiFi network
    Serial.printf("connecting to WiFi network \"%s\"\n", ssid);
    WiFi.begin(ssid, pass);
    while ( WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(2000);
    }
    Serial.printf("\n connected successfully to \"%s\". IP address: %s\n", ssid, WiFi.localIP().toString());

    Sound.init();
    Display.init();
    Device.init();

    Sound.connecttohost(RADIO_STREAM);
    Sound.setVolume(1);

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

        // change the neopixel color
    //    Device.setRotaryPixelColor(0,wheel(new_position & 0xFF));
        rotary_position = new_position;      // and save for next round
    }

    uint16_t x, y, z1, z2;
  if (Device.readTouch(&x, &y, &z1, &z2)) {
    Serial.print("Touch point: (");
    Serial.print(x); Serial.print(", ");
    Serial.print(y); Serial.print(", ");
    Serial.print(z1); Serial.print(" / ");
    Serial.print(z2); Serial.println(")");
  }

   delay(10); 
}



