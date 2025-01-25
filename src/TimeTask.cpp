#define ARDUINOJSON_ENABLE_PROGMEM 1
#include "TimeTask.h"
#include <ArduinoLog.h>
#include "time.h"
#include "esp_sntp.h"
#include "WirelessTask.h" // Include the header file for Wireless
#include "DisplayTask.h"
#include <ArduinoJson.h>
#include "Timezones.h"

TimeTask::TimeTask() 
{

}   

bool TimeTask::init() 
{
    char returnValueBuffer[40];
    char urlBuffer[400];
    DynamicJsonDocument doc(1024);
    deserializeJson(doc,timezones);

    if(Wireless.get("https://ipv4.iplocation.net/","ip", returnValueBuffer, 40))
    {
        Log.infoln("Got IP address: %s", returnValueBuffer);
        sprintf(urlBuffer, "https://www.timeapi.io/api/time/current/ip?ipAddress=%s", returnValueBuffer);

        if(Wireless.get(urlBuffer,"timeZone", returnValueBuffer,40))
        {
            Log.infoln("Got timezone: %s", returnValueBuffer);
            const char* timezzoneCode = doc[returnValueBuffer];

            if(timezzoneCode == NULL)
            {
                Log.errorln("Failed to get timezone code");
                return false;
            }

            Log.infoln("Got timezone code: %s", timezzoneCode);
            setClockSource(timezzoneCode);
        }
        else
        {
            Log.errorln("Failed to get timezone");
        }
    }
    else
    {
        Log.errorln("Failed to get IP address");
    }

    return true;
}

void TimeTask::tick() 
{
    static ulong lastTime = 0;
    char buffer[20];

    if(millis() - lastTime > 15000)
    {
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
        
        lastTime = millis();
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

TimeTask Time;