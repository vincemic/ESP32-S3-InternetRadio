#include <Arduino.h>
#include "Utilities.h"
#include <ArduinoLog.h>

size_t indexOf(const char* base, const char* str, size_t startIndex) {
  const char *p = base;
  for (; startIndex > 0; startIndex--)
      if (*p++ == '\0') return -1;
  char* pos = strstr(p, str);
  if (pos == nullptr) return -1;
  return pos - base;
};

size_t indexOfString(const char* buffer, size_t instanceNumber, size_t bufferSize) {
  size_t index = 0;
  size_t startIndex = 0;

  for (; instanceNumber > 0; instanceNumber--)
  {
    startIndex = index;

    while(index < bufferSize)
    {
      if(buffer[index] == '\0')
      {
        index++;
        break;
      }

      index++;
    }

    if(index >= bufferSize)
    {
      return -1;
    }

  }

  return startIndex;
    
};