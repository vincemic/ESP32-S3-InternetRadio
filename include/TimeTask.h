#pragma once

#include "ThreadTask.h"

class TimeTask : public ThreadTask {
public:
    TimeTask();
    static bool begin(String timezone);
    void tick();
    static void timeavailable(struct timeval *t);
    void setClockSource(const char* timezone);
    static void logTime();
    void updateClock();

private:
    bool start(String timezone);
    
};

extern TimeTask Time;