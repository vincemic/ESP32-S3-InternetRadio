#include <Arduino.h>
#include <ArduinoLog.h>
#include <lvgl.h>
#include <Audio.h>
#include "ui/ui_events.h"
#include "DisplayTask.h" 
#include "WirelessTask.h"
#include "ConfigurationTask.h"

int indexOf (const char* base, const char* str, int startIndex = 0) {
    const char *p = base;
    for (; startIndex > 0; startIndex--)
        if (*p++ == '\0') return -1;
    char* pos = strstr(p, str);
    if (pos == nullptr) return -1;
    return pos - base;
}

void keyboardReadyKey(lv_event_t * e)
{

    const char* ssid = lv_textarea_get_text(ui_Network_Screen_SSID_Text_Area);
    const char* password = lv_textarea_get_text(ui_Network_Screen_Password_Text_Area);
    Configuration.setWifiCredentials(ssid, password);
    Log.infoln("ssid: %s password: %s",ssid, password);

  
}

void cancelNetworkScreen(lv_event_t * e)
{
    lv_textarea_set_text(ui_Network_Screen_SSID_Text_Area, Configuration.getWifiSSID().c_str());
    lv_textarea_set_text(ui_Network_Screen_Password_Text_Area, Configuration.getWifiPassword().c_str());

}

void audio_id3data(const char *info){  //id3 metadata
    Log.infoln("id3data     %s",info);

}

void audio_eof_mp3(const char *info){  //end of file
    Log.infoln("eof_mp3     %s",info);
}

void audio_showstation(const char *info){
    Log.infoln("station     %s",info);

    if(info != NULL)
    {
        Display.send(DISPLAY_MESSAGE_STATION,info);
    }
}

void audio_showstreamtitle(const char *info){
    Log.infoln("streamtitle  %s",info);

    if(info != NULL)
    {
        size_t infoSize =strlen(info);
        int  artistSize = indexOf(info, " - ");
        if( artistSize > 0)
        {
            char* artist = (char *) malloc(artistSize +1);
            memcpy(artist, info, artistSize );
            artist[artistSize] = '\0';
            Display.send(DISPLAY_MESSAGE_ARTIST,artist);
            free(artist);
       

            int titleIndex = artistSize + 3;

            if(titleIndex < infoSize)
            {
                int titleSize = infoSize - titleIndex;
                char* title = (char *) malloc(titleSize + 1);
                memcpy(title, info + artistSize + 3, titleSize );
                title[titleSize] = '\0';
                Display.send(DISPLAY_MESSAGE_TITLE,title);
                free(title);
            }
        }
        else 
        {
            Display.send(DISPLAY_MESSAGE_COMMERCIAL,info);
        }
    }
}

void audio_bitrate(const char *info){
    Log.infoln("bitrate      %s",info);
}
void audio_commercial(const char *info){  //duration in sec
    Log.infoln("commercial   %s",info);
}
void audio_icyurl(const char *info){  //homepage
    Log.infoln("icyurl       %s",info);
}
void audio_lasthost(const char *info){  //stream URL played
    Log.infoln("lasthost     %s",info);
}
void audio_eof_speech(const char *info){
    Log.infoln("eof_speech   %s",info);
}
void audio_icylogo(const char* info) {
    Log.infoln("icylogo   %s",info);
}

void audio_id3image(fs::File& file, const size_t pos, const size_t size){
    Log.infoln("id3image  ");
}

void audio_oggimage(fs::File& file, std::vector<uint32_t> v){
    Log.infoln("oggimage  ");
}

void audio_id3lyrics(fs::File& file, const size_t pos, const size_t size){
    Log.infoln("id3lyrics  ");
}


void audio_icydescription(const char* info){
    Log.infoln("icydescription   %s",info);
}

