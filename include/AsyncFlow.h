#pragma once
#include <Arduino.h>
#include <vector>

#define flow_t AsyncFlow*
typedef bool (*asyncflow_cb)();
typedef bool (*asyncflow_text_cb)(const char* text);
typedef bool (*asyncflow_between_cb)();
typedef bool (*asyncflow_failure_cb)(const char* label);

struct AsyncFlowStep {
    uint32_t delay;
    asyncflow_cb callback;
    asyncflow_text_cb callbacktext;
    const char* label;
    uint32_t lastMillis = 0;
    uint16_t repeat = 0;
    const char * text;
};

class AsyncFlow {  

public:
    AsyncFlow(){};
    bool begin(asyncflow_between_cb between_cb = NULL, asyncflow_failure_cb failureCB = NULL);
    void addStep(const char* label,uint32_t delayMills, asyncflow_cb callback, uint16_t repeat = 0);
    void addStep(const char* label,uint32_t delayMills, asyncflow_text_cb callback, const char* text);
    bool tick();

private:
    std::vector<AsyncFlowStep*> steps;
    asyncflow_between_cb betweenStepCallback;
    asyncflow_failure_cb failureCallback;
    u_int16_t currentStep = 0;
 };

 class AsyncFlowConfiguration {
    public:
        AsyncFlowConfiguration(AsyncFlow* flow) {
            asyncFlow = flow;
        }

        AsyncFlowConfiguration* addStep(const char* label,uint32_t delayMills, asyncflow_cb callback, uint16_t repeat = 0) {
            asyncFlow->addStep(label,delayMills,callback,repeat);
            return this;
        }

        AsyncFlowConfiguration* addStep(const char* label,uint32_t delayMills, asyncflow_text_cb callback, const char* text) {
            asyncFlow->addStep(label,delayMills,callback, text);
            return this;
        }
        
    private:
        AsyncFlow* asyncFlow;
};