#pragma once
#include <Arduino.h>
#include "ThreadTask.h"
#include <Preferences.h>


class ConfigurationTask : public ThreadTask {
    public:
        ConfigurationTask();
        bool begin();
        void tick();
        String getWifiPassword();
        String getWifiSSID();
        void setWifiCredentials(const char* ssid, const char *password);
        void setLastStation(const char* station);
        String getLastStation();
        void setLastMode(const char* mode);
        String getLastMode();
        void setLastVolume(uint8_t volume);
        uint8_t getLastVolume();
        void setIPAddress(const char* ipAddress);
        String getIPAddress();
        void setTimeZone(const char* timeZone);
        String getTimeZone();
    private:
        Preferences preferences;

};
extern ConfigurationTask Configuration;