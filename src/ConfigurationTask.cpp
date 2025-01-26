#include "ConfigurationTask.h"
#include <Preferences.h>

#define PREFERENCE_NAME_SPACE "internet_radio"
#define PREFERENCE_WIFI_SSID "wifi_ssid"
#define PREFERENCE_WIFI_PASSWORD "wifi_password"
#define PREFERENCE_LAST_STATION "last_station"
#define PREFERENCE_LAST_MODE "last_mode"

#define RW_MODE false
#define RO_MODE true

ConfigurationTask::ConfigurationTask() {}

bool ConfigurationTask::init() {
    return preferences.begin(PREFERENCE_NAME_SPACE, RW_MODE); 
}

void ConfigurationTask::tick() {
    ThreadMessage message;
    if(internalReceive(&message))
    {
        switch (message.messageType)
        {
            default:
                break;
        }
    }
}

void ConfigurationTask::setWifiCredentials(const char* ssid, const char *password) {
    preferences.putString(PREFERENCE_WIFI_SSID, ssid);
    preferences.putString(PREFERENCE_WIFI_PASSWORD, password);

}

String ConfigurationTask::getWifiPassword() {
    return preferences.getString(PREFERENCE_WIFI_PASSWORD);
}

String ConfigurationTask::getWifiSSID() {
    return preferences.getString(PREFERENCE_WIFI_SSID);
}

void ConfigurationTask::setLastStation(const char* station) {
    preferences.putString(PREFERENCE_LAST_STATION, station);
}
String ConfigurationTask::getLastStation() {
    return preferences.getString(PREFERENCE_LAST_STATION);
}

void ConfigurationTask::setLastMode(const char *mode)
{
     preferences.putString(PREFERENCE_LAST_MODE, mode);
}

String ConfigurationTask::getLastMode()
{
   return preferences.getString(PREFERENCE_LAST_MODE);
}

ConfigurationTask Configuration;