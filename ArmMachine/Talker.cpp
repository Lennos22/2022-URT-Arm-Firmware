#include "Talker.h"
#include "BasicDefs.h"
#include "ArmMachine.h"
#include <Arduino.h>


Talker::Talker(){}

void Talker::begin(){
    Serial.begin(BAUD_RATE);
    begun = TRUE;

    if(VERBOSE) {
        Serial.println(); Serial.println();
        Serial.println("Started talker module");
    }
}

void Talker::sayHi(){
    if(begun){
        Serial.println("Hello");
    }
}

void Talker::sendMsg(uint32_t *words, uint8_t n){
    char sendChars[15];
    Serial.print("|");
    for(int i = 0; i<n; i++){
        sprintf(sendChars, "%08x", words[i]);
        Serial.print(sendChars);
        Serial.print("|");
    }
    Serial.println();

}

void Talker::word2chars(char *buffer, uint32_t num){
    for(int i = 0; i<8; i++){
        uint8_t temp = num && 0xf;
        if(temp<10){
            buffer[i] = (char) ('0'+temp);
        } else {
            buffer[i] = (char) ('a'+temp-10);
        }
        num >> 4;

    }
}