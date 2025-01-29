#include "OrchestratorTask.h"

OrchestratorTask::OrchestratorTask()
{
}

bool OrchestratorTask::begin()
{
    return Orchestrator.start();
}

void OrchestratorTask::tick()
{
    static AsyncFlow* currentFlow = NULL;

    if(currentFlow == NULL)
       xQueueReceive(flowQueue, &currentFlow, 0);
   
   
    if(currentFlow != NULL)
    {
        if(!currentFlow->tick())
        {
            delete currentFlow;
            currentFlow = NULL;
        }
    }
    
 
}

void OrchestratorTask::createAsyncFlow(asyncflow_configure_cb configureCB)
{
    if(configureCB != NULL)
    {
        AsyncFlow* flow = new AsyncFlow();
        AsyncFlowConfiguration flowConfig(flow);
        configureCB(&flowConfig);
        flow->begin();
        xQueueSend(flowQueue, &flow, 0);
    }
}


bool OrchestratorTask::start()
{
    return false;
}

OrchestratorTask Orchestrator;