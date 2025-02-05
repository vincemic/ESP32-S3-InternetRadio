#include "ServiceTask.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <esp_http_client.h>

#define CLOUD_MESSAGE_DOWNLOAD_STATIONS_INFO 1

class CloudTask : public ThreadTask
{
    public:
        CloudTask();
        static bool begin();
        void tick();
        void createTTSFile(const __FlashStringHelper *text,  const char *filePath);
        void getFile(const char *filePath);
        bool downloadIPAddress(String &ipAddress);
        bool downloadTimezone(String &ipAddress , String &timezone);
        // bool downloadStationsInfo2();  
        bool downloadStation(JsonDocument &stationJson, String &stationName);
        bool downloadStationList();  
        static esp_err_t http_event_handler(esp_http_client_event_t *evt);
    
    private:
        bool start();
        //bool clientReady = false;
        //esp_http_client_handle_t client;

};

extern CloudTask Cloud; 