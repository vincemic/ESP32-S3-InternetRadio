#include "OrchestratorTask.h"
#include <ArduinoLog.h>

JsonDocument OrchestratorTask::stationListJson = JsonDocument(&spiRamAllocator);
JsonDocument OrchestratorTask::stationJson = JsonDocument(&spiRamAllocator);
bool OrchestratorTask::initialized = false;
String OrchestratorTask::timezone ="";
String OrchestratorTask::ipAddress = "";

OrchestratorTask::OrchestratorTask()
{
}

bool OrchestratorTask::begin()
{
    return Orchestrator.start();
}

void OrchestratorTask::tick()
{
    static AsyncFlow* currentFlow = NULL;

    ThreadMessage message;
    if(internalReceive(&message))
    {
        
        switch (message.messageType)
        {
            case ORCHESTRATOR_START:
                if(!initialized)
                    startup();
                break;

            case ORCHESTRATOR_MESSAGE_STATION_SELECTED:
                break;
    
            default:
                break;
        }
        
    }

    if(currentFlow == NULL)
       xQueueReceive(flowQueue, &currentFlow, 0);
   
   
    if(currentFlow != NULL)
    {
        if(!currentFlow->tick())
        {
            delete currentFlow;
            currentFlow = NULL;
        }
    }
    
    if(!initialized) {
        Display.tick();  
    } 
    else {
        Device.tick();
        Sound.tick();
        Time.tick();
        CloudService.tick();
        Display.tick();
    }
 
}

void OrchestratorTask::startup()
{
      createAsyncFlow([](AsyncFlowConfiguration* flowConfig) {
        flowConfig->addStep("showscreen", 3000, showMainScreen)
        ->addStep("wifi", 500, Wireless.begin)
        ->addStep("waitwifi", 1000, Wireless.isWifiConnected, 5)
        ->addStep("devices", 40, Device.begin)
        ->addStep("sound", 40, Sound.begin)
        ->addStep("cloud", 40, CloudService.begin)
        ->addStep("ip", 40, getIPAddress)
        ->addStep("showip", 40, showIPAddress)
        ->addStep("timezone", 40, getTimezone)
        ->addStep("showtimezone", 40, showTimezone)
        ->addStep("time", 40, beginTime)
        ->addStep("showstations", 40, showGettingStations)
        ->addStep("stations", 40, getStations)
        ->addStep("loadstations", 40, getStations)
        ->addStep("partitions", 40, logPartitions)
        ->addStep("memory", 40, logMemory)
        ->addStep("radio", 40, startRadio)
        ->addStep("initialized", 40, setInitialized);

    });
}

void OrchestratorTask::createAsyncFlow(asyncflow_configure_cb configureCB)
{
    if(configureCB != NULL)
    {
        AsyncFlow* flow = new AsyncFlow();
        AsyncFlowConfiguration flowConfig(flow);
        configureCB(&flowConfig);
        flow->begin();
        xQueueSend(flowQueue, &flow, 0);
    }
}


bool OrchestratorTask::start()
{
    return false;
}

void OrchestratorTask::startStationSelecttionFlow()
{
    Orchestrator.createAsyncFlow([](AsyncFlowConfiguration* flowConfig) {
        flowConfig->addStep("showtuning", 40, showTuningScreen)
        ->addStep("showscreen", 3000, showTuningScreen);
    });
    
}

bool OrchestratorTask::showTuningScreen()
{
   return Display.send(DISPLAY_MESSAGE_TUNING_SCREEN);
}


bool OrchestratorTask::getStation(const char * stationName)
{
    return CloudService.getStation(stationListJson, stationName);
}


bool OrchestratorTask::logPartitions() {
    const esp_partition_t *partition;
    esp_partition_iterator_t iterator = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);

    if (iterator == nullptr) {
        Log.infoln(F("No partitions found."));
        return false;
    }

    Log.infoln(F("Partitions:"));
    while (iterator != nullptr) {
        partition = esp_partition_get(iterator);
        Log.infoln("Label: %s, Size: %lu bytes, Address: 0x%08lx",
                        partition->label,
                        partition->size,
                        partition->address);
        iterator = esp_partition_next(iterator);
    }

    esp_partition_iterator_release(iterator);  // Release the iterator once done

    return true;
}

bool OrchestratorTask::logMemory() {
    Log.infoln("Total heap: %d", ESP.getHeapSize());
    Log.infoln("Free heap: %d", ESP.getFreeHeap());
    Log.infoln("Total PSRAM: %d", ESP.getPsramSize());
    Log.infoln("Free PSRAM: %d", ESP.getFreePsram());

    return true;
}

bool OrchestratorTask::showMainScreen() {
    return Display.send(DISPLAY_MESSAGE_START, NULL);

}

bool OrchestratorTask::startRadio() {
    return Sound.send(SOUND_MESSAGE_CONNECT, RADIO_STREAM);
}

bool OrchestratorTask::getIPAddress() {
    return CloudService.getIPAddress(ipAddress);
}

bool OrchestratorTask::getTimezone() {
    return CloudService.getTimezone(ipAddress, timezone);
}

bool OrchestratorTask::beginTime() {
    return Time.begin(timezone);
}

bool OrchestratorTask::getStations(){
    return CloudService.getStationList(stationListJson);
}

bool OrchestratorTask::showIPAddress() {
    return Display.send(DISPLAY_MESSAGE_IPADDRESS, ipAddress.c_str());
}

bool OrchestratorTask::showTimezone() {
    return Display.send(DISPLAY_MESSAGE_TIMEZONE, timezone.c_str());
}

bool OrchestratorTask::showGettingStations() {
    return Display.send(DISPLAY_MESSAGE_STATION_LIST, "Getting stations");
}

bool OrchestratorTask::loadStations()
{
    String string;
    int i = 0;

    for (JsonArray::iterator it = stationListJson.as<JsonArray>().begin(); it != stationListJson.as<JsonArray>().end(); ++it) 
    {
        if(i < 50) 
        {

            string.concat( (*it).as<const char*>());
            string.concat("\n");
            i++;
        }
    }

    lv_roller_set_options(uic_Station_Selection_Screen_Roller, string.c_str(), LV_ROLLER_MODE_INFINITE);
    return true;
}

bool OrchestratorTask::setInitialized() {
    initialized = true;
    return true;
}

OrchestratorTask Orchestrator;