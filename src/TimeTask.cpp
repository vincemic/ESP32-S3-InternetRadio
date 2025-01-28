#define ARDUINOJSON_ENABLE_PROGMEM 1
#include "TimeTask.h"
#include <ArduinoLog.h>
#include "time.h"
#include "esp_sntp.h"
#include "WirelessTask.h" // Include the header file for Wireless
#include "DisplayTask.h"
#include <ArduinoJson.h>
#include "Timezones.h"
#include "CloudServiceTask.h"

TimeTask::TimeTask() 
{

}   

bool TimeTask::start(String timezone) 
{
    SpiRamAllocator allocator;
    JsonDocument doc = JsonDocument(&allocator);
    
    deserializeJson(doc,timezones);

    const char* timezzoneCode = doc[timezone];

    if(timezzoneCode == NULL)
    {
        Log.errorln("Failed to get timezone code");
        return false;
    }

    Log.infoln("Got timezone code: %s", timezzoneCode);
    setClockSource(timezzoneCode);
      

    return true;
}

bool TimeTask::begin(String timezone) 
{
    return Time.start(timezone);
}

void TimeTask::tick() 
{
    static uint32_t lastTime = 0;

    if(lastTime++ > 1500)
    {
        updateClock();
        
        lastTime = 0;
    }

    ThreadMessage message;
    if(internalReceive(&message))
    {
        switch (message.messageType)
        {
            default:
                break;
        }
    }


}

void TimeTask::timeavailable(struct timeval *t)
{

    Log.infoln("Got time adjustment from NTP!");
    Time.updateClock();
    logTime();

      
}

void TimeTask::setClockSource(const char * timezone)
{

    Log.infoln(F("Setting NTP time sync source"));

    // let us know when sync is done
    sntp_set_time_sync_notification_cb(timeavailable);

    configTime(0,0, "pool.ntp.org");
    setenv("TZ", timezone, 1); 
    tzset();

    Time.logTime();

}

void TimeTask::logTime()
{
    time_t now;                         
    struct tm timeinfo;

    time(&now); // read the current time
    localtime_r(&now, &timeinfo);   

    Log.infoln(F("Current system time: %s"), asctime(&timeinfo));
}

void TimeTask::updateClock()
{
    char buffer[20];
    time_t now;                         
    struct tm timeinfo;

    time(&now); // read the current time
    localtime_r(&now, &timeinfo);   
    size_t size = strftime(buffer, 20-1, "%I:%M %p", &timeinfo);
    buffer[size] = '\0';
    
    if(buffer[0] == '0')
    {
        buffer[0] = ' ';
    }

    Display.send(DISPLAY_MESSAGE_TTIME, buffer);
}
TimeTask Time;