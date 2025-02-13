#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>


struct SpiRamAllocator : public ArduinoJson::Allocator {
    void* allocate(size_t size) override {
      return ps_malloc(size);
    }

    void deallocate(void* pointer) override {
      free(pointer);
    }

    void* reallocate(void* ptr, size_t new_size) override {
      return ps_realloc(ptr, new_size);
    }
  };

size_t indexOf(const char* base, const char* str, size_t startIndex = 0);


