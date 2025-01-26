#pragma once

#include "ThreadTask.h"

class WirelessTask : public ThreadTask {

    public:
        WirelessTask();
        bool init();
        void tick();

        void sendPackCommand(const __FlashStringHelper *commandUri);
        bool get(const char *url, const char *jsonLabel, char * buffer,uint16_t bufferLength);
};

extern WirelessTask Wireless;