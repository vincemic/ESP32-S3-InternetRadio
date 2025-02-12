#include "ServiceTask.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <esp_http_client.h>

#define CLOUD_MESSAGE_DOWNLOAD_STATION_NAMES 1
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
        bool downloadStation(JsonDocument &stationJson, String &stationName);
        static esp_err_t http_event_handler(esp_http_client_event_t *evt);
    
    private:
        bool start();
        void downloadStationNames();  

};

extern CloudTask Cloud; 