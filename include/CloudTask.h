#include "ServiceTask.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <esp_http_client.h>

#define CLOUD_AMAZON_URL "https://bitdogttsbucket.s3.amazonaws.com"
#define CLOUD_STATIONS_URL "https://api.laut.fm/station_names"
#define CLOUD_STATION_URL "https://api.laut.fm/station"


#define CLOUD_STATION_LIST_FILE "/station_list.json"
#define CLOUD_STATION_FILE "/station.json"

#define CLOUD_MESSAGE_DOWNLOAD_STATION_NAMES 1
#define CLOUD_MESSAGE_DOWNLOAD_STATION_INFORMATION 2

class CloudTask : public ThreadTask
{
    public:
        CloudTask();
        static bool begin();
        void tick();
        void createTTSFile(const __FlashStringHelper *text,  const char *filePath);
        bool getFile(const char *filePath, const char *url);
        bool downloadIPAddress(String &ipAddress);
        bool downloadTimezone(String &ipAddress , String &timezone);
        bool downloadStation(JsonDocument &stationJson, String &stationName);
        static esp_err_t http_event_handler(esp_http_client_event_t *evt);
    
    private:
        bool start();

};

extern CloudTask Cloud; 