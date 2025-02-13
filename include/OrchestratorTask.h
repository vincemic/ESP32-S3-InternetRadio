#pragma once
#include <Arduino.h>
#include "AsyncFlow.h"
#include "ThreadTask.h"
#include "Utilities.h"



typedef void (*asyncflow_configure_cb)(AsyncFlowConfiguration* configuration);


#define ORCHESTRATOR_MESSAGE_SCREEM_CHANGED 2
#define ORCHESTRATOR_START 3
#define ORCHESTRATOR_MESSAGE_ROTARY_LEFT 4
#define ORCHESTRATOR_MESSAGE_ROTARY_RIGHT 5
#define ORCHESTRATOR_MESSAGE_ROTARY_PUSH 6
#define ORCHESTRATOR_MESSAGE_CURRENT_SCREEN 7
#define ORCHESTRATOR_MESSAGE_ERROR 8
#define ORCHESTRATOR_MESSAGE_WIFI_CONNECTED 10
#define ORCHESTRATOR_MESSAGE_WIFI_DISCONNECTED 11
#define ORCHESTRATOR_MESSAGE_UPDATE_CLOCK 12
#define ORCHESTRATOR_MESSAGE_NETOWRK_SAVE_CREDENTIALS 13
#define ORCHESTRATOR_MESSAGE_NETOWRK_CANCEL 14
#define ORCHESTRATOR_MESSAGE_STATION_LIST_DOWNLOADED 15
#define ORCHESTRATOR_MESSAGE_STATION_SELECTED_PLAY 16


#define MAX_STATION_SETS 500
#define STATION_SET_SIZE 50

#define STATION_LIST_FILE "/station_list.json"


class OrchestratorTask : public ThreadTask
{
public:
    OrchestratorTask();
    static bool begin();
    void tick();

    JsonDocument stationJson = JsonDocument(&spiRamAllocator);
    size_t stationSetIndexes[MAX_STATION_SETS];
    size_t stationSetCount = 0;
    char * stationNamesString = NULL;
    size_t stationSetStringsSize = 0;


private:
    bool start();
    QueueHandle_t flowQueue = xQueueCreate( 40, sizeof( AsyncFlow * ) );
    void startup();

    static void startStationSelectedFlow();

    static bool downloadStationInfo();
    static bool logPartitions();
    static bool logMemory();
    static bool startRadio();
    static bool downloadIPAddress();
    static bool downloadTimezone();
    static bool beginTime();
    static bool downloadStationNames();
 
    static bool showStationSelectionScreen();
    static bool showMessageScreen(const char* text);
    static bool showMessageScreen();
    static bool showIPAddress();
    static bool showTimezone();
    static bool showGettingStations();
    static bool areStationNamesDownloaded();
    static bool updateStationSelectionScreen();

    static bool setMessageScreenMessage(const char* message);
    static bool setInitialized();
    static bool createPagedStationNameList();


  
    

    void createAsyncFlow(asyncflow_configure_cb configureCB);
    void getStationSetIndexes();

    String timezone;
    String ipAddress;
    String stationName;
    bool newIPAdderss = false;
    bool initialized = false;
    SpiRamAllocator spiRamAllocator;
    bool stationListDownloaded = false;
    uint16_t currentStation = 0;

};

extern OrchestratorTask Orchestrator;