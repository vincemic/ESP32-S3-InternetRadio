#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <FS.h>
#include <SD.h> 
#include <Audio.h>
#include <esp_mac.h>
#include <TFT_eSPI.h>
#include "lvgl.h"
#include "ui/ui.h"
#include "SoundTask.h"
#include "DisplayTask.h"
#include "esp_mac.h"
#include "ThreadTask.h"

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

SPIClass *spi_onboardSD = new SPIClass(FSPI);
static ulong getMills() { return (esp_timer_get_time() / 1000LL); };

void setup()
{
    Serial.begin(115200);

    // FSPI default definitions for SS, MOSI, SCK & MISO are 10, 11, 12 & 13 
    // (see pins_arduino.h for more info) and match the boards wiring
    spi_onboardSD->begin();

    if (!SD.begin(SS, *spi_onboardSD)) {
        Serial.println("error mounting microSD");
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
    
    Sound.connecttohost(RADIO_STREAM);
    Sound.setVolume(1);
}



void loop() 
{
   vTaskDelay(100 * configTICK_RATE_HZ ); 
}


