#include "CloudTask.h"
#include <ArduinoLog.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <SD.h>
#include <DisplayTask.h>
#include <WirelessTask.h>
#include <OrchestratorTask.h>
#include <esp_crt_bundle.h>
#include <WiFiClient.h>
#include "Utilities.h"



CloudTask::CloudTask()
{
   
}

bool CloudTask::start()
{
   
    return true;
}

bool CloudTask::begin()
{
    return Cloud.start();
}

void CloudTask::tick()
{
    ThreadMessage threadMessage;
    if(internalReceive(&threadMessage))
    {
        
        switch (threadMessage.messageType)
        {
            case CLOUD_MESSAGE_DOWNLOAD_STATION_NAMES:
                Log.infoln("Download station names started");
                getFile(CLOUD_STATION_LIST_FILE, CLOUD_STATIONS_URL);
                Orchestrator.send(ORCHESTRATOR_MESSAGE_STATION_LIST_DOWNLOADED);
            break;
            case CLOUD_MESSAGE_DOWNLOAD_STATION_INFORMATION:
            
                Log.infoln("Download station information started for %s", threadMessage.message);
                String url(CLOUD_STATION_URL);
                url.concat('/');
                url.concat(threadMessage.message);
                getFile(CLOUD_STATION_FILE, url.c_str());
            
                Orchestrator.send(ORCHESTRATOR_MESSAGE_STATION_INFORMATION_DOWNLOADED);
            break;
        }
    }

}

bool CloudTask::getFile(const char *filePath, const char *url)
{
    WiFiClientSecure wifiClient;
    wifiClient.setInsecure();
    HTTPClient httpClient;
    bool result = false;
 
    Log.traceln(F("[HTTPS] begin..."));
    if (httpClient.begin(wifiClient, url))
    {
        httpClient.setTimeout(45000);
        // HTTPS
        Log.traceln(F("[HTTPS] GET..."));
        // start connection and send HTTP header
        int httpCode = httpClient.GET();


        // HTTP header has been send and Server response header has been handled
        Log.traceln(F("[HTTPS] GET... code: %d"), httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
        {
            Log.traceln(F("[HTTPS] Saving to file: %s"), filePath);
            File file = SD.open(filePath, FILE_WRITE, true);

            if (file) {
                httpClient.writeToStream(&file);
                file.close(); 
                result = true;  
                
                Log.errorln(F("[HTTPS] Saved file: %s"), filePath);
            }
            else 
            {
                Log.traceln(F("[HTTPS] Could not open SD file: %s"), filePath);
            }

        }
        else
        {
            Log.errorln(F("[HTTPS] GET... failed, error: %s"), httpClient.errorToString(httpCode).c_str());
        }

        httpClient.end();
    }
    else
    {
        Log.errorln(F("[HTTPS] Unable to create client"));
    }

    return result;
}

bool CloudTask::downloadIPAddress(String &ipAddress)
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

bool CloudTask::downloadTimezone(String &ipAddress , String &timezone)
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

void CloudTask::createTTSFile(const __FlashStringHelper *text,  const char *filePath)
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


bool CloudTask::downloadStation(JsonDocument &stationJson, String &stationName) {
    char url[400];
    WiFiClientSecure wifiClient;
    HTTPClient httpClient;
    wifiClient.setInsecure();
    httpClient.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    sprintf(url, "https://api.laut.fm/station/%s", stationName.c_str());
    const char * filePath = "/station.json";
    
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

            Log.traceln(F("[HTTPS] Saving to file: %s"), filePath);
            File file = SD.open(filePath, FILE_WRITE, true);

            if (file) 
            {
                httpClient.writeToStream(&file);
 
            }
            else {
                Log.traceln(F("[HTTPS] Could not open SD file: %s"), filePath);
                return false;
            }

            file.close();
            file = SD.open(filePath, FILE_READ);
            
            if(file) {
                DeserializationError error = deserializeJson(stationJson, file);

                if(error.code() == DeserializationError::Ok)
                {
                    result = true;
                }
                else
                {
                    Log.errorln(F("[HTTP] GET... failed, error parsing JSON code: %d"), error.code());
                }

            } else {
                Log.errorln(F("[HTTP] GET... failed, error opening file"));
            }
          
        }
        else
        {
             Log.errorln(F("[HTTP] GET... failed, code:%d-%s"), httpCode, httpClient.errorToString(httpCode).c_str());
        }
        
        httpClient.end();
    }
    else
    {
        Log.errorln(F("[HTTP] Unable to create client"));
    }



    return result;
}

CloudTask Cloud;