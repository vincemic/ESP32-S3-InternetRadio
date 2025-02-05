#pragma once
#include <Arduino.h>
#include "ThreadTask.h"

class WirelessTask : public ThreadTask {

    public:
        WirelessTask();
        static bool begin();
        void tick();
        //void sendPackCommand(const __FlashStringHelper *commandUri);
        bool get(const char *url, const char *jsonLabel, char * buffer,uint16_t bufferLength);
        static bool isWifiConnected();
    private:
        bool start();
};

extern WirelessTask Wireless;