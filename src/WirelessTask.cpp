#include "WirelessTask.h"
#include <Arduino.h>
#include <WiFi.h>
#include <DisplayTask.h>
#include <ESPmDNS.h>
#include <ArduinoLog.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "ConfigurationTask.h"

const char hostname[] = "ESPRadio"; 

WirelessTask::WirelessTask() {

}


bool WirelessTask::init() 
{

    WiFi.setHostname(hostname);
    String ssid = Configuration.getWifiSSID();
    uint16_t count = 0;

    // connecting to local WiFi network
    Log.infoln("connecting to WiFi network \"%s\"\n", ssid);
    WiFi.begin(ssid, Configuration.getWifiPassword());
    
    while ( WiFi.status() != WL_CONNECTED && count < 20) {
        Log.info(".");
        delay(2000);
        count++;
    }

    if(WiFi.status() != WL_CONNECTED)
    {
        Log.errorln("Failed to connect to WiFi");
        return false;
    }
    
    Display.send(DISPLAY_MESSAGE_WIFI_CONNECTED, "WiFi connected");
    Log.info("\n connected successfully to \"%s\". IP address: %s\n", ssid, WiFi.localIP().toString());

    MDNS.begin(hostname);

    return true;
}

void WirelessTask::tick() 
{
    ThreadMessage message;
    if(internalReceive(&message))
    {
        switch (message.messageType)
        {
            default:
                break;
        }
    }

    if(WiFi.status() != WL_CONNECTED)
    {
        Display.send(DISPLAY_MESSAGE_WIFI_DISCONNECTED, "WiFi disconnected");
    }
}


void WirelessTask::createTTSFile(const __FlashStringHelper *text,  const char *filePath)
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

void WirelessTask::getFile(const char *filePath)
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

void WirelessTask::sendPackCommand(const __FlashStringHelper *commandUri) {
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


bool WirelessTask::get(const char *url, const char *jsonLabel, char * buffer,uint16_t bufferLength) {
    WiFiClientSecure wifiClient;
    HTTPClient httpClient;
    wifiClient.setInsecure();
    httpClient.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    bool result = false;

    Log.traceln(F("[HTTP] begin %s"), url);

    if (httpClient.begin(wifiClient, url))
    {   
        httpClient.setTimeout(45000);
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

            JsonDocument doc;

            // Parse JSON object
            DeserializationError error = deserializeJson(doc, httpClient.getString());

            if(error.code() == DeserializationError::Ok)
            {
                const char* value = doc[jsonLabel].as<const char*>();

                Log.traceln(F("[HTTP] json value is: %s"), value);

                if(value != NULL)
                {
                    // get the size of the string or max buffer length
                    size_t size = strnlen(value, bufferLength-1);
                    // copy the string to the buffer
                    // filling the rest of the buffer with nulls
                    strncpy(buffer, value, size);
                    // null terminate the buffer, just in case
                    buffer[size] = '\0';

                    Log.traceln(F("[HTTP] GET... response: %s"), buffer);
                    result = true;
                }
                else
                {
                    Log.errorln(F("[HTTP] GET... failed, no value found"));
                }
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

WirelessTask Wireless;