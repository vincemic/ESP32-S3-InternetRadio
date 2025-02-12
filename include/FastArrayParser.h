#pragma once

#include <Arduino.h>
#include "ArduinoJson.h"
#include "Utilities.h"

class FastArrayParser
{   

    public:
        FastArrayParser(char * buffer, size_t bufferSize);
        ~FastArrayParser();
        bool process();

    private:
        char* buffer;
        size_t bufferSize = 0;


};

