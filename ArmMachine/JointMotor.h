#ifndef JointMotor_h
#define JointMotor_h

#include "BasicDefs.h"
#include <Arduino.h>
#include <TMCStepper.h>

#define MAX_STEPPERS 3

class JointMotor{
    public:
        void begin();
        void setSpeed(uint32_t newSpeed);
    private:
        uint8_t numMotors;
        uint32_t speed = 100000;
};

#endif