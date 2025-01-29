#pragma once
#include "AsyncFlow.h"

typedef void (*asyncflow_configure_cb)(AsyncFlowConfiguration* configuration);

class OrchestratorTask
{
public:
    OrchestratorTask();
    static bool begin();
    void tick();
    void createAsyncFlow(asyncflow_configure_cb configureCB);
private:
    bool start();
    QueueHandle_t flowQueue = xQueueCreate( 40, sizeof( AsyncFlow * ) );
};
extern OrchestratorTask Orchestrator;