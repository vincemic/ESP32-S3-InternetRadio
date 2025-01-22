#include <Arduino.h>
#include "SoundTask.h"
#include "DisplayTask.h"



int indexOf (const char* base, const char* str, int startIndex = 0) {
    const char *p = base;
    for (; startIndex > 0; startIndex--)
        if (*p++ == '\0') return -1;
    char* pos = strstr(p, str);
    if (pos == nullptr) return -1;
    return pos - base;
}

SoundTask::SoundTask() 
{
}

bool SoundTask::init()
{
    audio.setPinout(I2S_BCLK, I2S_LRCLK, I2S_DOUT);
    audio.setVolume(0); // 0...21
    audio.setConnectionTimeout(1200,0);  // needed for some stations esp. from around the globe

    xTaskCreatePinnedToCore(
        [](void *parameters)
        {
            while(true)
            {
                Sound.audio.loop();
                if (!Sound.audio.isRunning())
                {
                
                }
                vTaskDelay(40);
            }
        }
      ,             /* Function to implement the task */
        "audioTask",           /* Name of the task */
        10000,                  /* Stack size in words */
        NULL,                  /* Task input parameter */
        2 | portPRIVILEGE_BIT, /* Priority of the task */
        NULL,                  /* Task handle. */
        0                      /* Core where the task should run */
    );

    return true;
}

void SoundTask::setVolume(uint8_t vol)
{
    audio.setVolume(vol);

}

uint8_t SoundTask::getVolume()
{
    return audio.getVolume();
}

bool SoundTask::connecttohost(const char* host)
{
    return audio.connecttohost(host);
}

bool SoundTask::connecttoSD(const char* filename){
    return true;
}

void SoundTask::turnUpVolume()
{
    uint8_t vol = audio.getVolume();
    if(vol < 21)
    {
        vol++;
        audio.setVolume(vol);
    }
}

void SoundTask::turnDownVolume()
{
    uint8_t vol = audio.getVolume();
    if(vol > 0)
    {
        vol--;
        audio.setVolume(vol);
    }
}
SoundTask Sound;


void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);

}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);

    if(info != NULL)
    {
        Sound.send(DISPLAY_MESSAGE_STATION,info);
    }
}

void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle  ");Serial.println(info);

    if(info != NULL)
    {
        size_t infoSize =strlen(info);
        int  artistSize = indexOf(info, " - ");
        if( artistSize > 0)
        {
            char* artist = (char *) malloc(artistSize +1);
            memcpy(artist, info, artistSize );
            artist[artistSize] = '\0';
            Sound.send(DISPLAY_MESSAGE_ARTIST,artist);
            free(artist);
       

            int titleIndex = artistSize + 3;

            if(titleIndex < infoSize)
            {
                int titleSize = infoSize - titleIndex;
                char* title = (char *) malloc(titleSize + 1);
                memcpy(title, info + artistSize + 3, titleSize );
                title[titleSize] = '\0';
                Sound.send(DISPLAY_MESSAGE_TITLE,title);
                free(title);
            }
        }
    }
}

void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}
void audio_eof_speech(const char *info){
    Serial.print("eof_speech  ");Serial.println(info);
}
void audio_icylogo(const char* info) {
    Serial.print("icylogo  ");Serial.println(info);
}

void audio_id3image(fs::File& file, const size_t pos, const size_t size){
    Serial.print("id3image  ");
}

void audio_oggimage(fs::File& file, std::vector<uint32_t> v){
    Serial.print("oggimage  ");
}

void audio_id3lyrics(fs::File& file, const size_t pos, const size_t size){
    Serial.print("id3lyrics  ");
}


void audio_icydescription(const char* info){
    Serial.print("icydescription  ");Serial.println(info);
}