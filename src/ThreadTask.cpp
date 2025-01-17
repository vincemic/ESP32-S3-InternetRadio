#include "ThreadTask.h"

ThreadTask::ThreadTask() 
{
    
}

bool ThreadTask::init()
{


    return true;
}

bool ThreadTask::send(u16_t messageType, const char* info)
{
    // Creating a meesage that must be freed after it is received 
    ThreadMessage* outgoing = new ThreadMessage(messageType, info);  
    xQueueSend(rxQueue, &outgoing, portMAX_DELAY);
    return true;
}

bool ThreadTask::receive(ThreadMessage* message )
{
    ThreadMessage* incomming;

    if(xQueueReceive(txQueue, &incomming,portMAX_DELAY) == pdPASS) 
    {
        message->copy(incomming);
        // Fee the mewsage that was created
        free(incomming);
        return true;
    }

    return false;
}

bool ThreadTask::internalSend(u16_t messageType,const char* info)
{
    // Creating a meesage that must be freed after it is received 
    ThreadMessage* outgoing = new ThreadMessage(messageType, info);  

    xQueueSend(txQueue, &outgoing, portMAX_DELAY);

    return true;
}

bool ThreadTask::internalReceive(ThreadMessage* message )
{
    ThreadMessage* incomming;

    if(xQueueReceive(rxQueue, &incomming,portMAX_DELAY) == pdPASS) 
    {
        message->copy(incomming);

        // Fee the mewsage that was created
        free(incomming);
        return true;
    }

    return false;
}