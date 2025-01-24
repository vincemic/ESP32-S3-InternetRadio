#include "WirelessTask.h"
#include <Arduino.h>
#include <WiFi.h>
#include <DisplayTask.h>
#include <ESPmDNS.h>
#include <ArduinoLog.h>

const char ssid[] = "missile";
const char pass[] = "vincemic123!"; 
const char hostname[] = "ESPRadio"; 

WirelessTask::WirelessTask() {

}


bool WirelessTask::init() 
{

    WiFi.setHostname(hostname);

    // connecting to local WiFi network
    Log.infoln("connecting to WiFi network \"%s\"\n", ssid);
    WiFi.begin(ssid, pass);
    while ( WiFi.status() != WL_CONNECTED) {
        Log.info(".");
        delay(2000);
    }

    Display.send(DISPLAY_MESSAGE_WIFI_CONNECTED, "WiFi connected");
    Log.info("\n connected successfully to \"%s\". IP address: %s\n", ssid, WiFi.localIP().toString());

    MDNS.begin(hostname);

    return true;
}

void WirelessTask::tick() 
{
    ThreadMessage message;
    if(internalReceive(&message))
    {
        switch (message.messageType)
        {
            default:
                break;
        }
    }

    if(WiFi.status() != WL_CONNECTED)
    {
        Display.send(DISPLAY_MESSAGE_WIFI_DISCONNECTED, "WiFi disconnected");
    }
}

WirelessTask Wireless;