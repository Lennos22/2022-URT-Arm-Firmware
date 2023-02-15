#ifndef Listener_h
#define Listener_h

#include <Arduino.h>
#include "BasicDefs.h"

class Listener{
    public:
        void begin();
        void readMessage();

    private:
        uint32_t state;
        char receivedChars[CHARS_PER_MESSAGE];

};

#endif