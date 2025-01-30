#include "ServiceTask.h"
#include <Arduino.h>
#include <ArduinoJson.h>



class CloudServiceTask : public ServiceTask
{
    public:
        CloudServiceTask();
        static bool begin();
        void tick();
        void createTTSFile(const __FlashStringHelper *text,  const char *filePath);
        void getFile(const char *filePath);
        bool getIPAddress(String &ipAddress);
        bool getTimezone(String &ipAddress , String &timezone);
        bool getStationList(JsonDocument &stationListJson);  
        bool getStation(JsonDocument &stationJson, const char *stationName);
    
    private:
        bool start();

};

extern CloudServiceTask CloudService;