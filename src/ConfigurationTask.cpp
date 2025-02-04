#include "ConfigurationTask.h"
#include <Preferences.h>

#define PREFERENCE_NAME_SPACE "internet_radio"
#define PREFERENCE_WIFI_SSID "wifi_ssid"
#define PREFERENCE_WIFI_PASSWORD "wifi_password"
#define PREFERENCE_LAST_STATION "last_station"
#define PREFERENCE_LAST_MODE "last_mode"
#define PREFERENCE_LAST_VOLUME "last_volume"
#define PREFERENCE_IP_ADDRESS "ip_address"
#define PREFERENCE_TIME_ZONE "time_zone"


#define RW_MODE false
#define RO_MODE true

ConfigurationTask::ConfigurationTask() {}

bool ConfigurationTask::begin() {
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

void ConfigurationTask::setLastVolume(uint8_t volume)
{
    preferences.putUChar(PREFERENCE_LAST_VOLUME, volume);
}

uint8_t ConfigurationTask::getLastVolume()
{
    return preferences.getUChar(PREFERENCE_LAST_VOLUME);
}

void ConfigurationTask::setIPAddress(const char *ipAddress)
{
    preferences.putString(PREFERENCE_IP_ADDRESS, ipAddress);
}

String ConfigurationTask::getIPAddress()
{
    return preferences.getString(PREFERENCE_IP_ADDRESS);
}

void ConfigurationTask::setTimeZone(const char *timeZone)
{
    preferences.putString(PREFERENCE_TIME_ZONE, timeZone);
}

String ConfigurationTask::getTimeZone()
{
    return preferences.getString(PREFERENCE_TIME_ZONE);
}

ConfigurationTask Configuration;