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
#define ORCHESTRATOR_MESSAGE_MODE_SELECTED_RADIO 17
#define ORCHESTRATOR_MESSAGE_MODE_SELECTED_CLOCK 18
#define ORCHESTRATOR_MESSAGE_MODE_SELECTED_NETWORK 19
#define ORCHESTRATOR_MESSAGE_MODE_SELECTED_PROTON 20
#define ORCHESTRATOR_MESSAGE_MODE_SELECTED_GAME 21
#define ORCHESTRATOR_MESSAGE_STATION_INFORMATION_DOWNLOADED 22


#define MAX_STATION_SETS 500
#define STATION_SET_SIZE 50



class OrchestratorTask : public ThreadTask
{
public:
    OrchestratorTask();
    static bool begin();
    void tick();

    size_t stationSetIndexes[MAX_STATION_SETS];
    size_t stationSetCount = 0;
    char * stationNamesString = NULL;
    size_t stationSetStringsSize = 0;


private:
    bool start();
    QueueHandle_t flowQueue = xQueueCreate( 40, sizeof( AsyncFlow * ) );
    void startup();

    static void startStationSelectedFlow();

    static bool logPartitions();
    static bool logMemory();
    static bool startRadio();
    static bool downloadIPAddress();
    static bool downloadTimezone();
    static bool beginTime();
    static bool downloadStationNames();
    static bool loadStationNames();
    static bool downloadStationInfomation();
    static bool loadStationInformation();
 
    static bool showStationSelectionScreen();
    static bool showMessageScreen(const char* text);
    static bool showMessageScreen();
    static bool showIPAddress();
    static bool showTimezone();
    static bool showGettingStations();
    static bool isStationNamesDownloaded();
    static bool isStationInfomrationDownloaded();
    static bool updateStationSelectionScreen();
    static bool clearRadioScreen();

    static bool setMessageScreenMessage(const char* message);
    static bool setInitialized();



  
    

    void createAsyncFlow(asyncflow_configure_cb configureCB);
    void getStationSetIndexes();

    String timezone;
    String ipAddress;
    String stationName;
    bool newIPAdderss = false;
    bool initialized = false;
    bool stationsNamesDownloaded = false;
    bool stationInformationDownloaded = false;
    uint16_t currentStation = 0;

};

extern OrchestratorTask Orchestrator;