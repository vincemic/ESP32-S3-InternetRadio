#pragma once

#include "ThreadTask.h"

class TimeTask : public ThreadTask {
public:
    TimeTask();
    bool init();
    void tick();
    static void timeavailable(struct timeval *t);
    void setClockSource(const char* timezone);
    static void logTime();
    void updateClock();
    
};

extern TimeTask Time;