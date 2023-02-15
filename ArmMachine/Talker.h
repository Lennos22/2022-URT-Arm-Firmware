#ifndef Talker_h
#define Talker_h

#include "BasicDefs.h"
#include <Arduino.h>

class Talker{
    public:
        Talker();
        void begin();
        void sayHi();
        void sendMsg(uint32_t *words, uint8_t n);
    private:
        bool begun = FALSE;
        void word2chars(char *charBuffer, uint32_t num);

};

#endif
