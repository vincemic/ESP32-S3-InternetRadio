#include "ServiceTask.h"
#include <Arduino.h>
#include <ArduinoJson.h>





struct SpiRamAllocator : ArduinoJson::Allocator {
  void* allocate(size_t size) override {
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
  }

  void deallocate(void* pointer) override {
    heap_caps_free(pointer);
  }

  void* reallocate(void* ptr, size_t new_size) override {
    return heap_caps_realloc(ptr, new_size, MALLOC_CAP_SPIRAM);
  }
};

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