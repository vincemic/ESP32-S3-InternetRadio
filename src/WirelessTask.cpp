#include "WirelessTask.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoLog.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <SD.h>
#include <ArduinoJson.h>
#include "ConfigurationTask.h"
#include "OrchestratorTask.h"

const char hostname[] = "ESPRadio"; 

WirelessTask::WirelessTask() {

}


bool WirelessTask::start() 
{

    WiFi.setHostname(hostname);
    String ssid = Configuration.getWifiSSID();

    // connecting to local WiFi network
    Log.infoln("connecting to WiFi network \"%s\"\n", ssid);
    WiFi.begin(ssid, Configuration.getWifiPassword());
    

    return true;
}

bool WirelessTask::begin() 
{
    return Wireless.start();
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

    isWifiConnected();
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

bool WirelessTask::isWifiConnected()
{
    bool result = WiFi.status() == WL_CONNECTED;

    if(result)
        Orchestrator.send(ORCHESTRATOR_MESSAGE_WIFI_CONNECTED);
    else    
        Orchestrator.send(ORCHESTRATOR_MESSAGE_WIFI_DISCONNECTED);

    return  result;
}

WirelessTask Wireless;