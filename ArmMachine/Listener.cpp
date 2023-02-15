#include "Listener.h"
#include "BasicDefs.h"
#include "ArmMachine.h"

boolean newData = false;

void Listener::begin(){
    if(VERBOSE) Serial.println("Started Listener module");
}

void Listener::readMessage(){
    if(VERBOSE) Serial.println("Read function");
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= CHARS_PER_MESSAGE) {
                ndx = CHARS_PER_MESSAGE - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }


    if(newData) {
        if(Serial.available()){
            newData=FALSE;
            readMessage();
            return;

        }
        newData = FALSE;
        Serial.print("I heard: ");
        Serial.println(receivedChars);
    }

}