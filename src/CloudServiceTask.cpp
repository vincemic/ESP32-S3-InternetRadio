#include "CloudServiceTask.h"
#include <ArduinoLog.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <DisplayTask.h>
#include <WirelessTask.h>




CloudServiceTask::CloudServiceTask()
{
   
}
bool CloudServiceTask::start()
{
   
    return true;
}

bool CloudServiceTask::begin()
{
    return CloudService.start();
}

void CloudServiceTask::tick()
{
    
}

void CloudServiceTask::getFile(const char *filePath)
{
    WiFiClientSecure wifiClient;
    wifiClient.setInsecure();
    HTTPClient httpClient;
    String path = String(F("https://bitdogttsbucket.s3.amazonaws.com"));

    path.concat(filePath);

    Log.traceln(F("[HTTPS] begin..."));
    if (httpClient.begin(wifiClient, path))
    {
        httpClient.setTimeout(45000);
        // HTTPS
        Log.traceln(F("[HTTPS] GET..."));
        // start connection and send HTTP header
        int httpCode = httpClient.GET();

        // httpCode will be negative on error
        if (httpCode > 0)
        {
            // HTTP header has been send and Server response header has been handled
            Log.traceln(F("[HTTPS] GET... code: %d"), httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
            {
                Log.traceln(F("[HTTPS] Saving to file: %s"), filePath);
                File file = SD.open(filePath, FILE_WRITE, true);

                if (file)
                    httpClient.writeToStream(&file);
                else
                    Log.traceln(F("[HTTPS] Could not open SD file: %s"), filePath);

                file.close();    
            }
        }
        else
        {
            Log.errorln(F("[HTTPS] GET... failed, error: %s"), httpClient.errorToString(httpCode).c_str());
        }

        Log.errorln(F("[HTTPS] Saved file: %s"), filePath);
    }
    else
    {
        Log.errorln(F("[HTTPS] Unable to create client"));
    }

    httpClient.end();
}

bool CloudServiceTask::getIPAddress(String &ipAddress)
{
    char returnValueBuffer[40];

    if(Wireless.get("https://ipv4.iplocation.net/","ip", returnValueBuffer, 40))
    {
        Log.infoln("Got IP address: %s", returnValueBuffer);
        ipAddress.concat(returnValueBuffer);
        return true;

    } else {
        Log.errorln("Failed to get IP address");
        return false;
    }
}

bool CloudServiceTask::getTimezone(String &ipAddress , String &timezone)
{
    char urlBuffer[400];
    char returnValueBuffer[40];

    sprintf(urlBuffer, "https://www.timeapi.io/api/time/current/ip?ipAddress=%s", ipAddress.c_str());

    if(Wireless.get(urlBuffer,"timeZone", returnValueBuffer,40))
    {
        Log.infoln("Got timezone: %s", returnValueBuffer);
        timezone.concat(returnValueBuffer);
        return true;
    }
    else
    {
        Log.errorln("Failed to get timezone");
        return false;

    }
}

void CloudServiceTask::createTTSFile(const __FlashStringHelper *text,  const char *filePath)
{
    JsonDocument jsonDocument;
    WiFiClientSecure wifiClient;
    wifiClient.setInsecure();
    HTTPClient https;
    String path = String(F("https://cq3odfu0dg.execute-api.us-east-1.amazonaws.com/Production/"));
    String json;
    String label = String(filePath);
    
    label.replace(F(".mp3"),F(""));
    label.replace(F("/"),F(""));

    jsonDocument["text"] = text;
    jsonDocument["label"] = label;
    serializeJson(jsonDocument, json);

    Log.traceln(F("[HTTPS] begin..."));
    if (https.begin(wifiClient, path))
    {
        https.setTimeout(45000);
        // HTTPS
        Log.traceln(F("[HTTPS] POST..."));
        // start connection and send HTTP header
        int httpCode = https.POST(json);

        // httpCode will be negative on error
        if (httpCode == HTTP_CODE_OK)
        {
            // HTTP header has been send and Server response header has been handled
            Log.traceln(F("[HTTPS] POST... code: %d"), httpCode);
        }
        else
        {
            Log.errorln(F("[HTTPS] POST... failed, code: %s"), https.errorToString(httpCode).c_str());
        }
    }
    else
    {
        Log.errorln(F("[HTTPS] Unable to create client"));
    }

    https.end();
}

bool CloudServiceTask::getStationList(JsonDocument &stationListJson) {
    WiFiClientSecure wifiClient;
    HTTPClient httpClient;
    wifiClient.setInsecure();
    httpClient.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    const char * url= "https://api.laut.fm/station_names";
    bool result = false;

    Log.traceln(F("[HTTP] begin %s"), url);

    if (httpClient.begin(wifiClient, url))
    {   
        // set 5 minute timeout
        httpClient.setTimeout(37856);
        httpClient.addHeader("accept", "application/json");

        // HTTPS
        Log.traceln(F("[HTTP] GET %s"), url);
        // start connection and send HTTP header
        int httpCode = httpClient.GET();

        // httpCode will be negative on error
        if (httpCode == HTTP_CODE_OK)
        {
            // HTTP header has been send and Server response header has been handled
            // getSize may not work with chunked encoding
            Log.traceln(F("[HTTP] GET... code: %d content size %d"), httpCode, httpClient.getSize());

            // Parse JSON object
            DeserializationError error = deserializeJson(stationListJson, httpClient.getString());

            if(error.code() == DeserializationError::Ok)
            {
                result = true;

            } else {
                Log.errorln(F("[HTTP] GET... failed, error parsing JSON code: %d"), error.code());
            }
        }
        else
        {
             Log.errorln(F("[HTTP] GET... failed, code:%d-%s"), httpCode, httpClient.errorToString(httpCode).c_str());
        }
    }
    else
    {
        Log.errorln(F("[HTTP] Unable to create client"));
    }

    httpClient.end();

    return result;
}

bool CloudServiceTask::getStation(JsonDocument &stationListJson, const char *stationName) {
    WiFiClientSecure wifiClient;
    HTTPClient httpClient;
    wifiClient.setInsecure();
    httpClient.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    String url= "https://api.laut.fm/station/";
    url.concat(stationName);
    bool result = false;

    Log.traceln(F("[HTTP] begin %s"), url);

    if (httpClient.begin(wifiClient, url))
    {   
        // set 5 minute timeout
        httpClient.setTimeout(37856);
        httpClient.addHeader("accept", "application/json");

        // HTTPS
        Log.traceln(F("[HTTP] GET %s"), url);
        // start connection and send HTTP header
        int httpCode = httpClient.GET();

        // httpCode will be negative on error
        if (httpCode == HTTP_CODE_OK)
        {
            // HTTP header has been send and Server response header has been handled
            // getSize may not work with chunked encoding
            Log.traceln(F("[HTTP] GET... code: %d content size %d"), httpCode, httpClient.getSize());

            // Parse JSON object
            DeserializationError error = deserializeJson(stationListJson, httpClient.getString());

            if(error.code() == DeserializationError::Ok)
            {
                result = true;

            } else {
                Log.errorln(F("[HTTP] GET... failed, error parsing JSON code: %d"), error.code());
            }
        }
        else
        {
             Log.errorln(F("[HTTP] GET... failed, code:%d-%s"), httpCode, httpClient.errorToString(httpCode).c_str());
        }
    }
    else
    {
        Log.errorln(F("[HTTP] Unable to create client"));
    }

    httpClient.end();

    return result;
}

CloudServiceTask CloudService;