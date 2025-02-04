#include "RecordProcessor.h"
#include <ArduinoLog.h>
#include "Utilities.h"

#define RECORD_PROCESSOR_BUFFER_SIZE 1024 * 1




RecordProcessor::RecordProcessor(JsonDocument &stationsJson) 
{
    stationsJson = stationsJson;
    stationJson = JsonDocument(&spiRamAllocator);
    buffer = (char *) spiRamAllocator.allocate(RECORD_PROCESSOR_BUFFER_SIZE);

}

bool RecordProcessor::process(const char *chunk, size_t size)
{
    DeserializationError error;
    size_t objectStartIndex = 0;

    for(size_t chunkIndex = 0; chunkIndex < size; chunkIndex++)
    { 
        char c = chunk[chunkIndex];

        switch(c)
        {   
            case '\"':

                if(escapeCharacter && textStartCount == 1)
                {
                    escapeCharacter = false;

                } else {
                   
                   textStartCount++;

                    if(textStartCount == 2)
                        textStartCount = 0;
                }

                buffer[bufferIndex] = c;
                bufferIndex++;
                break;
            case '[':
                if(escapeCharacter && textStartCount == 1)
                {
                    escapeCharacter = false;
                    buffer[bufferIndex] = c;
                    bufferIndex++;
                } else {
                    
                    arrayStartCount++;
                    if(objectStartCount > 0 )
                    {
                        buffer[bufferIndex] = c;
                        bufferIndex++;
                    }
                }
                break;
            case '{':
                if(escapeCharacter && textStartCount == 1)
                {
                    escapeCharacter = false;
                }
                else {
                    objectStartCount++;
                }

                buffer[bufferIndex] = c;
                bufferIndex++;
        
                break;
            case '}':
                buffer[bufferIndex] = c;
                bufferIndex++;
                if(escapeCharacter && textStartCount == 1)
                {
                    escapeCharacter = false;
                }
                else
                {
                    objectStartCount--;

                    if(objectStartCount == 0)
                    {
                        buffer[bufferIndex] = '\0';
                        objectStartIndex = indexOf((const char *)buffer, "{");


                        error = deserializeJson(stationJson, buffer + objectStartIndex);

                        if(error == DeserializationError::Ok)
                        {
                            sprintf(buffer, "{\"name\":\"%s\", \"display\": \"%s\"}", stationJson["name"].as<const char*>(), stationJson["display_name"].as<const char*>());
                            error = deserializeJson(stationJson, buffer);

                            if(error == DeserializationError::Ok) {
                                //stationsJson.add(stationJson.as<JsonObject>());
                            }
                            else
                            {
                                Log.infoln("Station: %s",(const char *) buffer);
                                Log.errorln("Failed to new station info");
                            }

                        } 
                        else 
                        {
                            Log.infoln("Station: %s",(const char *) buffer + objectStartIndex);
                            Log.errorln("Failed to parse station");
                            return false;
                        }

                        bufferIndex = 0;
                        stationJson.clear();

                    }
                }

                break;
            case ']':
                if(escapeCharacter && textStartCount == 1)
                {
                    escapeCharacter = false;
                }
                else {
                    arrayStartCount--;

                    if (arrayStartCount == 0)
                    {
                        return false;
                    }

                }

                buffer[bufferIndex] = c;
                bufferIndex++;
                break;
            case '\\':
                if(textStartCount == 1 && !escapeCharacter)
                    escapeCharacter = true;
                else
                    escapeCharacter = false;

                buffer[bufferIndex] = c;
                bufferIndex++;   
                break;
            default:
                buffer[bufferIndex] = c;
                bufferIndex++;
                break;
        }   
        
    }

    return true;
}

RecordProcessor::~RecordProcessor()
{
   spiRamAllocator.deallocate(buffer);
}


int indexOf2 (const char* base, const char* str, int startIndex = 0) {
  const char *p = base;
  for (; startIndex > 0; startIndex--)
      if (*p++ == '\0') return -1;
  char* pos = strstr(p, str);
  if (pos == nullptr) return -1;
  return pos - base;
};