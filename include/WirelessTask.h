#pragma once

#include "ThreadTask.h"

class WirelessTask : public ThreadTask {

    public:
        WirelessTask();
        bool init();
        void tick();
};

extern WirelessTask Wireless;