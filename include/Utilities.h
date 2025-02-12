#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>


struct SpiRamAllocator : public ArduinoJson::Allocator {
    void* allocate(size_t size) override {
      return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
    }

    void deallocate(void* pointer) override {
      heap_caps_free(pointer);
    }

    void* reallocate(void* ptr, size_t new_size) override {
      return heap_caps_realloc(ptr, new_size, MALLOC_CAP_SPIRAM);
    }
  };

size_t indexOf(const char* base, const char* str, size_t startIndex = 0);


