#include "AsyncFlow.h"
#include <Arduino.h>
#include <ArduinoLog.h>


bool AsyncFlow::begin(asyncflow_between_cb betweenStepCB, asyncflow_failure_cb failureCB) {
    this->betweenStepCallback = betweenStepCB;
    this->failureCallback = failureCB;
    return true;
}
  

void AsyncFlow::addStep(const char* label, uint32_t delayMills, asyncflow_cb cb, uint16_t repeat) {
    AsyncFlowStep* step = new AsyncFlowStep();
    step->delay = delayMills;
    step->callback = cb;
    step->label = label;
    step->repeat = repeat;

    steps.push_back(step);
}

void AsyncFlow::addStep(const char *label, uint32_t delayMills, asyncflow_text_cb callback, const char *text)
{
    AsyncFlowStep* step = new AsyncFlowStep();
    step->delay = delayMills;
    step->callbacktext = callback;
    step->label = label;
    step->text = text;

    steps.push_back(step);
}

bool AsyncFlow::tick() {

    if(betweenStepCallback != NULL) {
        betweenStepCallback();
    }

    if(currentStep >= steps.size()) {
        return false;
    }

    AsyncFlowStep* step = steps[currentStep];

    // delay running this step as defined
    if(step->lastMillis == 0) {
        step->lastMillis = millis() + step->delay;
    }

    // break for now until the delay is over
    if(millis() < step->lastMillis) {
        return true;
    }

    Log.infoln("Running step %s", step->label);

    bool stepResult = false;
    
    if(step->callback != NULL)
        stepResult = step->callback();
    else if(step->callbacktext != NULL)
        stepResult = step->callbacktext(step->text);

    if(!stepResult && step->repeat < 1) {
        Log.infoln("Step failed: %s", step->label);
        failureCallback(step->label);
        // set to the end of the steps
        currentStep = steps.size();
        return false;
    } else if(!stepResult && step->repeat > 1) {    
        step->lastMillis = 0;
        step->repeat--;

        // repeat the step if was not successful
        // keep repeating until the repeat count is 0
        return true;
    } 


    currentStep++;
    return true;
}

