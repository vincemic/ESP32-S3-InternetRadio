#include <Arduino.h>
#include <ArduinoLog.h>
#include <lvgl.h>
#include <Audio.h>
#include "ui/ui_events.h"
#include "DisplayTask.h" 
#include "WirelessTask.h"
#include "ConfigurationTask.h"
#include "OrchestratorTask.h"
#include "Utilities.h"

void modeSelected(lv_event_t * e)
{
    char mode[200];
    lv_roller_get_selected_str(uic_Mode_Screen_Mode_Roller, mode, 200);
    Log.infoln("Selected mode: %s",mode);

    switch(mode[0])
    {
        case 'R':
            Orchestrator.send(ORCHESTRATOR_MESSAGE_MODE_SELECTED_RADIO);
            break;
        case 'C':
            Orchestrator.send(ORCHESTRATOR_MESSAGE_MODE_SELECTED_CLOCK);
            break;
        case 'N':
            Orchestrator.send(ORCHESTRATOR_MESSAGE_MODE_SELECTED_NETWORK);
            break;
        case 'P':
            Orchestrator.send(ORCHESTRATOR_MESSAGE_MODE_SELECTED_PROTON);
            break;
        case 'G':
            Orchestrator.send(ORCHESTRATOR_MESSAGE_MODE_SELECTED_GAME);
            break;
    }
}

void keyboardReadyKey(lv_event_t * e)
{
    Orchestrator.send(ORCHESTRATOR_MESSAGE_NETOWRK_SAVE_CREDENTIALS);
}

void cancelNetworkScreen(lv_event_t * e)
{
    Orchestrator.send(ORCHESTRATOR_MESSAGE_NETOWRK_CANCEL);
}

void stationSelectionChanged(lv_event_t * e) {
    Display.send(DISPLAY_MESSAGE_UPDATE_STATIONS);
}

void playStation(lv_event_t * e) {
    Orchestrator.send(ORCHESTRATOR_MESSAGE_STATION_SELECTED_PLAY);
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
    if(info != NULL && strlen(info) > 0)
    {
        Display.send(DISPLAY_MESSAGE_COMMERCIAL,info);
    }

    Log.infoln("icydescription   %s",info);
}

