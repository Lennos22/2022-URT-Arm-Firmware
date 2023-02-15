#ifndef ArmMachine_h
#define ArmMachine_h

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#include "BasicDefs.h"
#include "Talker.h"
#include "Listener.h"
#include "JointEncoder.h"
#include "JointMotor.h"


class ArmMachine{
    public:
        void begin();
        void run();
    private:
        Talker talker;
        Listener listener;
        JointEncoder joints;
        JointMotor motors;
        
        uint32_t state;
        uint32_t lastRun;

        bool timeGuard();



};

#endif