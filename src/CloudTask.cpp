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
            case CLOUD_MESSAGE_DOWNLOAD_STATION_LIST:
                Log.infoln("Downloaded stations info started");
                    downloadStationNames();
                
            break;
        }
    }

}

void CloudTask::getFile(const char *filePath)
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

void CloudTask::downloadStationNames() {

    WiFiClientSecure wifiClient;
    wifiClient.setInsecure();
    HTTPClient httpClient;
    String path = String(F("https://api.laut.fm/station_names"));
    const char * filePath = "/stations.json";

    const char* keys[] = {"Transfer-Encoding"};
    httpClient.collectHeaders(keys, 1);

    Log.traceln(F("[HTTPS] begin..."));
    if (httpClient.begin(wifiClient, path))
    {
        httpClient.setReuse(false);
        httpClient.setTimeout(30000);
        httpClient.setConnectTimeout(30000);
        // Set the Accept header to application/json
        httpClient.addHeader("Accept", "application/json"); 

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
                {
                    httpClient.writeToStream(&file);
                }
                else
                    Log.traceln(F("[HTTPS] Could not open SD file: %s"), filePath);

                file.close();    
                
                file = SD.open(filePath, FILE_READ);

                if (file) 
                {
                    Log.traceln("Reading file: %s size: %d", filePath, file.size());

                    // Because we are removing json formatting, the data in the buffer
                    // will be smaller than the file size
                    size_t bufferSize = file.size();
                    char * buffer = (char *) ps_malloc(bufferSize);
                    size_t index = 0;
                    char * currentValue;
                    size_t currentLength = 0;
                    size_t itemCounter = 0;
                    size_t setCounter = 0;

                    DeserializationError error = deserializeJson(Orchestrator.stationListJson, file);

                    if(error.code() == DeserializationError::Ok)
                    {
                        JsonArray jsonArray = Orchestrator.stationListJson.as<JsonArray>();
                        for(JsonVariant value : jsonArray)
                        {
                            currentValue = (char *) value.as<const char*>();
                            currentLength = strlen(currentValue);

                            memcpy(buffer + index, currentValue, currentLength);
                            buffer[index + currentLength] = '\n';
                            index += currentLength + 1;
                            itemCounter++;

                            // count to every 50th item
                            // terminate the string with null instead of newline
                            if(itemCounter % 50 == 0)
                            {
                                buffer[index -1] = '\0';
                                setCounter++;
                            }
                        }

                        if(itemCounter % 50 != 0) {
                            setCounter++;
                            // termiate the remaining set of items    
                            buffer[index -1] = '\0';
                        }

                        Log.traceln("Read %d items, %d sets", itemCounter, setCounter);

                        for(size_t i = 1; i <= setCounter; ++i)
                        {
                            Log.traceln("Set: %d index: %d", i, indexOfString(buffer, i, bufferSize));

                            Log.traceln("Set %d: %s", i, buffer + indexOfString(buffer, i, bufferSize));
                        }
                    }
                    else
                    {
                        Log.errorln(F("[HTTPS] error parsing JSON code: %d"), error.code());
                    }

                }

                file.close();
            }
        }
        else
        {
            Log.errorln(F("[HTTPS] GET... failed, error: %s"), httpClient.errorToString(httpCode).c_str());
        }

    }
    else
    {
        Log.errorln(F("[HTTPS] Unable to create client"));
    }

    Orchestrator.send(ORCHESTRATOR_MESSAGE_STATION_LIST_DOWNLOADED);

}

bool CloudTask::downloadStation(JsonDocument &stationListJson, String &stationName) {
    char url[400];
    WiFiClientSecure wifiClient;
    HTTPClient httpClient;
    wifiClient.setInsecure();
    httpClient.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    sprintf(url, "https://api.laut.fm/station/%s", stationName.c_str());
    
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

CloudTask Cloud;