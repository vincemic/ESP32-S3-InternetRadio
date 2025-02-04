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
            case CLOUD_MESSAGE_DOWNLOAD_STATIONS_INFO:
                Cloud.downloadStationsInfo2();
                Log.infoln("Downloaded stations info started");
            break;
        }
    }

    if(client != NULL)
    {
        esp_err_t result = esp_http_client_perform(client);
        
        switch(result)
        {
            case ESP_OK:
                Log.infoln("HTTP request success");
                break;
            case ESP_FAIL:
                Log.errorln("HTTP request failed");
                esp_http_client_close(client);
                esp_http_client_cleanup(client);
                client = NULL;
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

bool CloudTask::downloadStationsInfo() {
    WiFiClientSecure wifiClient;
    wifiClient.setInsecure();
    HTTPClient httpClient;
    String path = String(F("https://api.laut.fm/station_names"));
    const char * filePath = "/stations.json";

    Log.traceln(F("[HTTPS] begin..."));
    if (httpClient.begin(wifiClient, path))
    {
        httpClient.setTimeout(30000);
        httpClient.setConnectTimeout(30000);
        // Set the Accept header to application/json
        httpClient.addHeader("Accept", "application/json");
        httpClient.addHeader("Transfer-Encoding", "Chunked");   

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

                if (file) {
  //                  String content = httpClient.getString();
   //                 file.write((const uint8_t*)content.c_str(), content.length());
                    httpClient.writeToStream(&file);    
                }
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

    return true;
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

bool CloudTask::downloadStationsInfo2()
{
    Log.infoln("Starting download client");

    esp_http_client_config_t config = {
        .url = "https://api.laut.fm/stations",
        .auth_type = HTTP_AUTH_TYPE_NONE,
        .method = HTTP_METHOD_GET,
        .timeout_ms = 180000,
        .event_handler = Cloud.http_event_handler,
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
        .buffer_size = 1024 * 8,
        .buffer_size_tx = 1024,
        .is_async = true,
        .crt_bundle_attach = esp_crt_bundle_attach,
        .keep_alive_enable = true,
    };
    
    client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Accept", "application/json");

    return true;

}

esp_err_t CloudTask::http_event_handler(esp_http_client_event_t *evt)
{
  static RecordProcessor processor(Orchestrator.stationListJson);
   File file;

  switch (evt->event_id) {
        case HTTP_EVENT_ERROR:
            Log.infoln("HTTP Event Error");
            esp_http_client_close(evt->client);
            esp_http_client_cleanup(evt->client);
            Cloud.client = NULL;
            break;
        case HTTP_EVENT_ON_CONNECTED:
            Log.infoln("HTTP Event On Connected");
            break;
        case HTTP_EVENT_HEADER_SENT:
           Log.infoln("HTTP Event Header Sent");
            break;
        case HTTP_EVENT_ON_HEADER:
            Log.infoln("HTTP Event Header: %s: %s\n", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            file = SD.open("/stations.json", FILE_APPEND);
            if(file)
            {
                file.write((const uint8_t*)evt->data, evt->data_len);
                file.close();
            }
            /*
            if(!processor.process((const char *)evt->data, evt->data_len))
            {

                esp_http_client_close(evt->client);
                esp_http_client_cleanup(evt->client);
                Cloud.client = NULL;
            }*/
            break;
        case HTTP_EVENT_ON_FINISH:
            Log.infoln("HTTP Event On Finish");
            if(Cloud.client != NULL) 
            {       
                esp_http_client_close(evt->client);
                esp_http_client_cleanup(evt->client);
                Cloud.client = NULL;
            }
            break;
        case HTTP_EVENT_DISCONNECTED:
            Log.infoln("HTTP Event Disconnected");
            if(Cloud.client != NULL) {
                esp_http_client_close(evt->client);
                esp_http_client_cleanup(evt->client);
                Cloud.client = NULL;
            }
            break;
    }
    return ESP_OK;
};

CloudTask Cloud;