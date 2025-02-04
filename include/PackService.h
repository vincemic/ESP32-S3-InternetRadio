#pragma once
#include <Arduino.h>
#include <WString.h>


class PackService
{
    public:
        PackService();
        bool begin();

        void sendPackCommand(const __FlashStringHelper *commandUri);
};

extern PackService Pack;