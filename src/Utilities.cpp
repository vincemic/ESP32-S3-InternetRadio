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




