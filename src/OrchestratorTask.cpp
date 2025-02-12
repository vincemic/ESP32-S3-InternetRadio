#include "OrchestratorTask.h"
#include <ArduinoLog.h>
#include "DisplayTask.h"
#include "CloudTask.h"
#include "SoundTask.h"
#include "ConfigurationTask.h"
#include "DeviceTask.h"
#include "WirelessTask.h"
#include "TimeTask.h"




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

    ThreadMessage threadMessage;
    if(internalReceive(&threadMessage))
    {
        
        switch (threadMessage.messageType)
        {
            case ORCHESTRATOR_START:
                if(!initialized)
                    startup();
                break;
            case ORCHESTRATOR_MESSAGE_WIFI_DISCONNECTED:
                Display.send(DISPLAY_MESSAGE_WIFI_DISCONNECTED);
                Display.send(DISPLAY_MESSAGE_SCREEN_MESSAGE_MESSAGE, "Disconnected from WiFi");
                break;
            case ORCHESTRATOR_MESSAGE_WIFI_CONNECTED:
                Display.send(DISPLAY_MESSAGE_WIFI_CONNECTED);
                Display.send(DISPLAY_MESSAGE_SCREEN_MESSAGE_MESSAGE, "Connected to WiFi");
                break;  
            case ORCHESTRATOR_MESSAGE_ROTARY_LEFT:
               if(Display.getActiveScreen() == ui_Station_Selection_Screen)
                {
                     Display.send(DISPLAY_MESSAGE_TUNE_DOWN);
                } 
                else if(Display.getActiveScreen() == ui_Main_Screen )
                {
                    Sound.send(SOUND_MESSAGE_TURN_DOWN_VOLUME);
                }
                break;
            case ORCHESTRATOR_MESSAGE_ROTARY_RIGHT:
                if(Display.getActiveScreen() == ui_Station_Selection_Screen)
                {
                     Display.send(DISPLAY_MESSAGE_TUNE_UP);
                } 
                else if(Display.getActiveScreen() == ui_Main_Screen )
                {
                    Sound.send(SOUND_MESSAGE_TURN_UP_VOLUME);
                }
                break;
            case ORCHESTRATOR_MESSAGE_ROTARY_PUSH:
                if(Display.getActiveScreen() == ui_Station_Selection_Screen)
                {
                    Orchestrator.send(ORCHESTRATOR_MESSAGE_STATION_SELECTED_PLAY);
                }
                break;
            case ORCHESTRATOR_MESSAGE_UPDATE_CLOCK:
                Display.send(DISPLAY_MESSAGE_UPDATE_CLOCK, threadMessage.message);       
    
                break;
            case ORCHESTRATOR_MESSAGE_NETOWRK_SAVE_CREDENTIALS:
            {
                const char* ssid = lv_textarea_get_text(ui_Network_Screen_SSID_Text_Area);
                const char* password = lv_textarea_get_text(ui_Network_Screen_Password_Text_Area);
                Configuration.setWifiCredentials(ssid, password);
                Log.infoln("ssid: %s password: %s",ssid, password);
            }
                break;
            case ORCHESTRATOR_MESSAGE_NETOWRK_CANCEL:
                lv_textarea_set_text(ui_Network_Screen_SSID_Text_Area, Configuration.getWifiSSID().c_str());
                lv_textarea_set_text(ui_Network_Screen_Password_Text_Area, Configuration.getWifiPassword().c_str());
                break;
            case ORCHESTRATOR_MESSAGE_STATION_SELECTED:

                break;
            case ORCHESTRATOR_MESSAGE_STATION_LIST_DOWNLOADED:
                stationListDownloaded = true;
                break;
            case ORCHESTRATOR_MESSAGE_STATION_SELECTED_PLAY:
                startStationSelecttionFlow();
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
        Cloud.tick();
        Display.tick();  
    } 
    else {
        Device.tick();
        Sound.tick();
        Time.tick();

        Display.tick();
    }
 
}


void OrchestratorTask::startup()
{
      createAsyncFlow([](AsyncFlowConfiguration* flowConfig) {
        // Wait three seconds before showing the message screen
        flowConfig->addStep("showscreen", 3000, showMessageScreen)
        // Wait 500ms before starting the wifi connection
        ->addStep("wifi beign", 500, Wireless.begin)
        // Wait 1000ms for the wifi to connect - rety 5 times
        ->addStep("wait for wifi connect", 1000, Wireless.isWifiConnected, 5)
        // Wait 500ms before starting devices
        ->addStep("starting devices", 40, Device.begin)
        // Wait 500ms before starting the sound
        ->addStep("string sound", 40, Sound.begin)
        // Wait 500ms before starting the cloud services
        ->addStep("string cloud services", 40, Cloud.begin)
        // Wait 500ms before starting downloading the ip address for internet
        ->addStep("download ip address", 40, downloadIPAddress)
        // Wait 500ms before showing the ip address
        ->addStep("show ip address on message screen", 40, showIPAddress)
        // Wait 500ms before downloading the timezone
        ->addStep("download timezone", 500, downloadTimezone)
        // Wait 500ms before showing the timezone
        ->addStep("show timezone", 40, showTimezone)
        // Wait 2000ms before starting the time service
        ->addStep("starting time service", 2000, beginTime)
        // Wait 40ms before downloading the stations info
        ->addStep("show getting stations", 40, showGettingStations)
        ->addStep("download station names", 40, downloadStationNames)
        // Wait 1000ms for the station list to be downloaded, repeat 100 times if nessary
        ->addStep("wait for station list", 1000, areStationNamesDownloaded, 100)
        ->addStep("creating paged station list", 40, createPagedStationNameList)
        ->addStep("update station selection screen", 40, updateStationSelectionScreen)
        ->addStep("partitions", 40, logPartitions)
        ->addStep("memory", 40, logMemory)
        // Wait 5000ms before showing radio screen
        ->addStep("start radio", 5000, startRadio)
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
    String stationName = Display.getSelectedStation();
    Log.infoln("Station selected %s", stationName.c_str());

    Orchestrator.createAsyncFlow([](AsyncFlowConfiguration* flowConfig) {
        flowConfig->addStep("download station info", 40, downloadStationInfo)
        ->addStep("start radio", 40, startRadio);
        
    });
    
}

bool OrchestratorTask::areStationNamesDownloaded()
{
    return Orchestrator.stationListDownloaded;
}

bool OrchestratorTask::updateStationSelectionScreen()
{
    Display.send(DISPLAY_MESSAGE_UPDATE_STATIONS);
    return true;
}


bool OrchestratorTask::downloadStationInfo()
{
    String stationName = Display.getSelectedStation();
    Log.infoln("Downloading station info for %s", stationName.c_str());
    
    if(Cloud.downloadStation(Orchestrator.stationJson,stationName)) {
        const char * streamUrl = Orchestrator.stationJson["stream_url"].as<const char *>();
        Configuration.setLastStation(streamUrl);
        return true;
    }
        
    return false;
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

bool OrchestratorTask::showMessageScreen() {
    return Display.send(DISPLAY_MESSAGE_SCREEN_MESSAGE);
}

bool OrchestratorTask::setMessageScreenMessage(const char* message) {
    return Display.send(DISPLAY_MESSAGE_SCREEN_MESSAGE_MESSAGE, message);
}

bool OrchestratorTask::showStationSelectionScreen() {
    return Display.send(DISPLAY_MESSAGE_SCREEN_STATION_SELECTION);
}

bool OrchestratorTask::startRadio() {
    String streamUrl = Configuration.getLastStation();

    return Display.send(DISPLAY_MESSAGE_SCREEN_RADIO) &&
    Sound.send(SOUND_MESSAGE_CONNECT, streamUrl.c_str());
}

bool OrchestratorTask::downloadIPAddress() {
    Display.send(DISPLAY_MESSAGE_SCREEN_MESSAGE_MESSAGE, "Getting IP address");
    Cloud.downloadIPAddress(Orchestrator.ipAddress);
    
    if(Orchestrator.ipAddress == Configuration.getIPAddress())  
     {
        Orchestrator.newIPAdderss = false;
     }   
    else
    {
        Configuration.setIPAddress(Orchestrator.ipAddress.c_str());
        Orchestrator.newIPAdderss = true;
    }
    
    return true;
}

bool OrchestratorTask::downloadTimezone() {
    if(Orchestrator.newIPAdderss || Configuration.getTimeZone().length() == 0)
    {
        Cloud.downloadTimezone(Orchestrator.ipAddress, Orchestrator.timezone);
        Configuration.setTimeZone(Orchestrator.timezone.c_str());    
    }    
    else
    {
        Orchestrator.timezone = Configuration.getTimeZone();
    }
    return true;
}

bool OrchestratorTask::beginTime() {
    return Time.begin(Orchestrator.timezone);
}

bool OrchestratorTask::downloadStationNames(){
    return Cloud.send(CLOUD_MESSAGE_DOWNLOAD_STATION_NAMES);
}

bool OrchestratorTask::showIPAddress() {
    return Display.send(DISPLAY_MESSAGE_SCREEN_MESSAGE_MESSAGE, Orchestrator.ipAddress.c_str());
}

bool OrchestratorTask::showTimezone() {
    return Display.send(DISPLAY_MESSAGE_SCREEN_MESSAGE_MESSAGE, Orchestrator.timezone.c_str());
}

bool OrchestratorTask::showGettingStations() {
    return Display.send(DISPLAY_MESSAGE_SCREEN_MESSAGE_MESSAGE, "Getting stations");
}


bool OrchestratorTask::setInitialized() {
    Orchestrator.initialized = true;
    return true;
}

bool OrchestratorTask::createPagedStationNameList()
{
    File file = SD.open(STATION_LIST_FILE, FILE_READ);
    
    if (file) 
    {
        Log.traceln("Reading file: %s size: %d", STATION_LIST_FILE, file.size());

        // Because we are removing json formatting, the data in the buffer
        // will be smaller than the file size
        size_t bufferSize = file.size();

        // Allocate a buffer to hold the station names
        // Each station name is separated by a newline character
        Orchestrator.stationNamesString = (char *) ps_malloc(bufferSize);

        size_t index = 0;
        size_t startIndex = 0;
        char * stationName;
        size_t currentLength = 0;
        size_t stationCounter = 0;
        Orchestrator.stationSetCount = 0;
        SpiRamAllocator spiRamAllocator;
        JsonDocument stationListJson(&spiRamAllocator);;

        // Deserialize the json file that was downloaded previosly
        DeserializationError error = deserializeJson(stationListJson, file);

        if(error.code() == DeserializationError::Ok)
        {
            JsonArray jsonArray = stationListJson.as<JsonArray>();

            // iterate over the array of station names
            for(JsonVariant value : jsonArray)
            {
                // Get thestation name from the array
                stationName = (char *) value.as<const char*>();

                // find its size
                currentLength = strlen(stationName);
                
                // Take the string value from the json array and copy it to the char * buffer
                memcpy(Orchestrator.stationNamesString + index, stationName, currentLength);
                
                // Add a newline character to the end of the station name in the char * buffer
                Orchestrator.stationNamesString[index + currentLength] = '\n';

                // move the buffer index to the end of the station name + new line character
                index += currentLength + 1;

                // add to the station counter
                stationCounter++;

                // if we have enough stations to make a set
                if(stationCounter % STATION_SET_SIZE == 0)
                {
                    // Capture the start index of the set
                    Orchestrator.stationSetIndexes[Orchestrator.stationSetCount] = startIndex;

                    // Capture the start index of the next set
                    startIndex = index;

                    // increment the station set count
                    Orchestrator.stationSetCount++;
                }
            }
            
            // There left over stations for an incomplete set
            if(stationCounter % STATION_SET_SIZE != 0) {

                // Capture the start index of the last incomplete set
                Orchestrator.stationSetIndexes[Orchestrator.stationSetCount] = startIndex;
                // increment the station set count
                Orchestrator.stationSetCount++; 

            }
            
            // terminate the large buffer string    
            if(index > 0) {
                Orchestrator.stationNamesString[index -1] = '\0';
                Orchestrator.stationSetStringsSize = index;

                // The last set index is would the start of the next set, if there was one. Now its just a terminator
                Orchestrator.stationSetIndexes[Orchestrator.stationSetCount] = index;  
            }

            Log.traceln("Read %d stations, %d station sets", stationCounter, Orchestrator.stationSetCount);

        }
        else
        {
            Log.errorln(F("[HTTPS] error parsing JSON code: %d"), error.code());
        }

        file.close();
    }
   
    return true;
}

OrchestratorTask Orchestrator;