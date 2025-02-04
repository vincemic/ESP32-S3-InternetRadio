#include "PackService.h"
#include <ArduinoLog.h>
#include <WiFiClientSecure.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DisplayTask.h>


PackService::PackService()
{
    
}

bool PackService::begin()
{
    return true;
}


void PackService::sendPackCommand(const __FlashStringHelper *commandUri) 
{
    WiFiClient wifiClient;
    HTTPClient httpClient;
    String contents = String(F(""));
    String path = String(F("http://192.168.5.240")) + String(F(commandUri));

    Log.traceln(F("[HTTP] begin..."));
    if (httpClient.begin(wifiClient, path))
    {
        httpClient.setTimeout(45000);
        // HTTPS
        Log.traceln(F("[HTTP] POST..."));
        // start connection and send HTTP header
        int httpCode = httpClient.PUT(contents.c_str());

        // httpCode will be negative on error
        if (httpCode == HTTP_CODE_OK)
        {
            // HTTP header has been send and Server response header has been handled
            Log.traceln(F("[HTTP] PUT... code: %d"), httpCode);
        }
        else
        {
            Log.errorln(F("[HTTP] PUT... failed, code: %s"), httpClient.errorToString(httpCode).c_str());
        }
    }
    else
    {
        Log.errorln(F("[HTTP] Unable to create client"));
    }

    httpClient.end();
}

PackService Pack;