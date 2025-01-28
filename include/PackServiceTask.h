#pragma once
#include "ServiceTask.h"

class PackServiceTask : public ServiceTask
{
    public:
        PackServiceTask();
        bool begin();
        void tick();
        void sendPackCommand(const __FlashStringHelper *commandUri);
};

extern PackServiceTask PackService;