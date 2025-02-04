#pragma once
#include <Arduino.h>
#include "AsyncFlow.h"
#include "ThreadTask.h"
#include "RecordProcessor.h"
#include "Utilities.h"


typedef void (*asyncflow_configure_cb)(AsyncFlowConfiguration* configuration);

#define ORCHESTRATOR_MESSAGE_STATION_SELECTED 1
#define ORCHESTRATOR_MESSAGE_SCREEM_CHANGED 2
#define ORCHESTRATOR_START 3
#define ORCHESTRATOR_MESSAGE_ROTARY_LEFT 4
#define ORCHESTRATOR_MESSAGE_ROTARY_RIGHT 5
#define ORCHESTRATOR_MESSAGE_ROTARY_PUSH 6
#define ORCHESTRATOR_MESSAGE_CURRENT_SCREEN 7
#define ORCHESTRATOR_MESSAGE_ERROR 8
#define ORCHESTRATOR_MESSAGE_WIFI_CONNECTING 9
#define ORCHESTRATOR_MESSAGE_WIFI_CONNECTED 10
#define ORCHESTRATOR_MESSAGE_WIFI_DISCONNECTED 11
#define ORCHESTRATOR_MESSAGE_UPDATE_CLOCK 12
#define ORCHESTRATOR_MESSAGE_NETOWRK_SAVE_CREDENTIALS 13
#define ORCHESTRATOR_MESSAGE_NETOWRK_CANCEL 14



class OrchestratorTask : public ThreadTask
{
public:
    OrchestratorTask();
    static bool begin();
    void tick();
    JsonDocument stationListJson = JsonDocument(&spiRamAllocator);;
    JsonDocument stationJson = JsonDocument(&spiRamAllocator);

private:
    bool start();
    QueueHandle_t flowQueue = xQueueCreate( 40, sizeof( AsyncFlow * ) );
    void startup();

    static bool getStationUrl();
    static void startStationSelecttionFlow();
    static bool setStationName();
    static bool downloadStationInfo();
    static bool logPartitions();
    static bool logMemory();
    static bool startRadio();
    static bool downloadIPAddress();
    static bool downloadTimezone();
    static bool beginTime();
    static bool getStation(const char * stationName);
    static bool downloadStationsInfo();
    static bool loadStationSelection();
    static bool showIPAddress();
    static bool showTimezone();
    static bool showGettingStations();
    static bool setInitialized();
    static bool showMessageScreen();
    static bool setMessageScreenMessage(const char* message);
    static bool showStationSelectionScreen();
    static bool showStationName();

  
    

    void createAsyncFlow(asyncflow_configure_cb configureCB);
    String timezone;
    String ipAddress;
    String stationName;
    bool newIPAdderss = false;
    bool initialized = false;
    SpiRamAllocator spiRamAllocator;
};

extern OrchestratorTask Orchestrator;