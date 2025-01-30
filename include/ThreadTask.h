#pragma once
#include <Arduino.h>
#include "ThreadTask.h"

#define MaxThreadMessageSize 250

struct ThreadMessage
{
    u16_t messageType = 0;
    char message[MaxThreadMessageSize];

    ThreadMessage()
    {

    }

    ThreadMessage(u16_t type, const char * info)
    {
        messageType = type;

        if(info == NULL)
        {
            message[0] = '\0';
        }
        else
        {
          size_t infosize = strnlen(info, MaxThreadMessageSize -1);
          strncpy(message, info, infosize);
          message[infosize] = '\0';
        }
    }

    void copy(ThreadMessage* source)
    {
      if(source->message == NULL)
      {
        message[0] = '\0';
      } 
      else{
        size_t infosize = strnlen(source->message, MaxThreadMessageSize -1);
        strncpy(message, source->message, infosize);
        message[infosize] = '\0';
      }

        messageType = source->messageType;

    }

};

class ThreadTask 
{

  public:
    ThreadTask();

    // Public endpoints
    bool send(u16_t messageType, const char* message);
    bool send(u16_t messageType);
    //bool receive(ThreadMessage* message);

  public:

   // bool internalSend(u16_t messageType, const char* message);
    bool internalReceive(ThreadMessage* message);
    

  private:
    QueueHandle_t rxQueue = xQueueCreate( 40, sizeof( struct TaskMessage * ) );
   // QueueHandle_t txQueue = xQueueCreate( 40, sizeof( struct TaskMessage * ) ); 
};

