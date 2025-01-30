#pragma once
#include "AsyncFlow.h"
#include "ThreadTask.h"
#include "DisplayTask.h"
#include "CloudServiceTask.h"
#include "SoundTask.h"
#include "ConfigurationTask.h"
#include "DeviceTask.h"
#include "WirelessTask.h"
#include "TimeTask.h"

typedef void (*asyncflow_configure_cb)(AsyncFlowConfiguration* configuration);

#define ORCHESTRATOR_MESSAGE_STATION_SELECTED 1
#define ORCHESTRATOR_MESSAGE_SCREEM_CHANGED 2
#define ORCHESTRATOR_START 3

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
} spiRamAllocator;

class OrchestratorTask : public ThreadTask
{
public:
    OrchestratorTask();
    static bool begin();
    void tick();


private:
    bool start();
    QueueHandle_t flowQueue = xQueueCreate( 40, sizeof( AsyncFlow * ) );
    void startup();

    static void startStationSelecttionFlow();
    static bool showTuningScreen();
    static bool getStation();
    static bool logPartitions();
    static bool logMemory();
    static bool showMainScreen();
    static bool startRadio();
    static bool getIPAddress();
    static bool getTimezone();
    static bool beginTime();
    static bool getStation(const char * stationName);
    static bool getStations();
    static bool loadStations();
    static bool showIPAddress();
    static bool showTimezone();
    static bool showGettingStations();
    static bool setInitialized();
    

    void createAsyncFlow(asyncflow_configure_cb configureCB);
    static String timezone;
    static String ipAddress;
    static JsonDocument stationListJson;
    static JsonDocument stationJson;
    static bool initialized;
};

extern OrchestratorTask Orchestrator;