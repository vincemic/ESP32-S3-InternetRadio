#pragma once
#include <Arduino.h>
#include <vector>

typedef bool (*asyncflow_cb)();
typedef bool (*asyncflow_failure_cb)(const char* label);

struct AsyncFlowStep {
    uint32_t delay;
    asyncflow_cb callback;
    const char* label;
    uint32_t lastMillis = 0;
    uint16_t repeat = 0;
};

class AsyncFlow {  

public:
    AsyncFlow(){};
    bool begin(asyncflow_cb betweenStepCB, asyncflow_failure_cb failureCB);
    void addStep(const char* label,uint32_t delayMills, asyncflow_cb callback, uint16_t repeat = 0);
    bool tick();

private:
    std::vector<AsyncFlowStep*> steps;
    asyncflow_cb betweenStepCallback;
    asyncflow_failure_cb failureCallback;
    u_int16_t currentStep = 0;
 };