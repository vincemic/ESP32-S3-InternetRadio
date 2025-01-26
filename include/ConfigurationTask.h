#pragma once
#include <Arduino.h>
#include "ThreadTask.h"
#include <Preferences.h>


class ConfigurationTask : public ThreadTask {
    public:
        ConfigurationTask();
        bool init();
        void tick();
        String getWifiPassword();
        String getWifiSSID();
        void setWifiCredentials(const char* ssid, const char *password);
        void setLastStation(const char* station);
        String getLastStation();
        void setLastMode(const char* mode);
        String getLastMode();
    private:
        Preferences preferences;
};
extern ConfigurationTask Configuration;