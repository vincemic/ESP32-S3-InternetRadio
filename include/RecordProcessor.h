#pragma once

#include <Arduino.h>
#include "ArduinoJson.h"
#include "Utilities.h"

class RecordProcessor
{   

    public:
        RecordProcessor(JsonDocument &jsonDocument);
        ~RecordProcessor();
        bool process(const char* chunk, size_t size);

    private:
        char* buffer;
        size_t bufferIndex = 0;
        size_t arrayStartCount = 0;
        size_t objectStartCount = 0;
        size_t textStartCount = 0;
        JsonDocument stationsJson; 
        JsonDocument stationJson;
        SpiRamAllocator spiRamAllocator;
        bool escapeCharacter = false;

};

