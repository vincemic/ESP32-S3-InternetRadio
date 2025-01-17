#pragma once
#include <Arduino.h>
#include <esp_mac.h>
#include "ThreadTask.h"

#define MaxThreadMessageSize 250

struct ThreadMessage
{
    u16_t messageType;
    char message[MaxThreadMessageSize];

    ThreadMessage()
    {

    }

    ThreadMessage(u16_t type, const char * info)
    {
        messageType = type;
        size_t infosize = strnlen(info, MaxThreadMessageSize -1);
        strncpy(message, info, infosize);
        message[infosize] = '\0';
    }

    void copy(ThreadMessage* source)
    {
        size_t infosize = strnlen(source->message, MaxThreadMessageSize -1);
        strncpy(message, source->message, infosize);
        message[infosize] = '\0';

        messageType = source->messageType;

    }

};

class ThreadTask 
{

  public:
    ThreadTask();
    virtual bool init();

    // Public endpoints
    bool send(u16_t messageType, const char* message);
    bool receive(ThreadMessage* message);

  public:

    bool internalSend(u16_t messageType, const char* message);
    bool internalReceive(ThreadMessage* message);
    

  private:
    QueueHandle_t rxQueue = xQueueCreate( 10, sizeof( struct TaskMessage * ) );
    QueueHandle_t txQueue = xQueueCreate( 10, sizeof( struct TaskMessage * ) ); 
};

