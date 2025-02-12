#include "FastArrayParser.h"
#include <ArduinoLog.h>


// Turn a json array string into a single string with each element separated by a newline character
FastArrayParser::FastArrayParser(char * buffer, size_t bufferSize)
{
    this->buffer = buffer;
    this->bufferSize = bufferSize;
}


bool FastArrayParser::process()
{
    size_t bufferReadIndex = 0;
    size_t bufferWriteIndex = 0;
    size_t arrayStartCount = 0;
    size_t objectStartCount = 0;
    size_t textStartCount = 0;
    bool escapeCharacter = false;
    size_t objectStartIndex = 0;

    for(; bufferReadIndex < bufferSize; bufferReadIndex++)
    { 
        char c = buffer[bufferReadIndex];

        switch(c)
        {   
            case '\"':

                if(escapeCharacter && textStartCount == 1)
                {
                    escapeCharacter = false;
                    buffer[bufferWriteIndex] = c;
                    bufferWriteIndex++;

                } else {
                   
                   textStartCount++;

                    if(textStartCount == 2)
                    {
                        textStartCount = 0;
                        buffer[bufferWriteIndex] = '\n';
                        bufferWriteIndex++;
                    }
                }


                break;
            case '[':
                if(escapeCharacter)
                {
                    escapeCharacter = false;
                } 

                if(textStartCount == 1)
                {
                    buffer[bufferWriteIndex] = c;
                    bufferWriteIndex++;
                } 
                else
                {
                    arrayStartCount++;
                }
                break;
            case '{':
                if(escapeCharacter)
                {
                    escapeCharacter = false;
                } 

                if(textStartCount == 1)
                {
                    buffer[bufferWriteIndex] = c;
                    bufferWriteIndex++;
                } 
                else
                {
                    objectStartCount++;
                }
        
                break;
            case '}':

                if(escapeCharacter)
                {
                    escapeCharacter = false;
                } 

                if(textStartCount == 1)
                {
                    buffer[bufferWriteIndex] = c;
                    bufferWriteIndex++;
                } 
                else
                {
                    objectStartCount--;
                }

                break;
            case ']':
                if(escapeCharacter)
                {
                    escapeCharacter = false;
                } 

                if(textStartCount == 1)
                {
                    buffer[bufferWriteIndex] = c;
                    bufferWriteIndex++;
                } 
                else
                {
                    arrayStartCount--;

                    if (arrayStartCount == 0)
                    {
                        buffer[bufferWriteIndex] = '\0';
                        bufferWriteIndex++;
                        return true;
                    }

                }


                break;
            case '\\':
                if(textStartCount == 1)
                {
                    if(!escapeCharacter)
                    {
                        escapeCharacter = true;
                    }
                    else
                    {
                        escapeCharacter = false;
                    }

                    buffer[bufferWriteIndex] = c;
                    bufferWriteIndex++;   
                }
                break;
            default:
                if(textStartCount == 1)
                {
                    buffer[bufferWriteIndex] = c;
                    bufferWriteIndex++;
                }
                break;
        }   
        
    }

    return false;
}

FastArrayParser::~FastArrayParser()
{
   
}

