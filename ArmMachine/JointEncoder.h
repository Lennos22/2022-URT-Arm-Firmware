#ifndef JointEncoder_h
#define JointEncoder_h

#include "BasicDefs.h"
#include "./src/EncoderAux/PCA9548A.h"
#include "AS5600.h"
#include <Arduino.h>



class JointEncoder{
    public:
        void begin();
        uint8_t getNumJoints();
        void getJointAngles(uint16_t raw_angles);
        uint32_t getJointAngle(uint8_t port);
        void getJointAngles(uint32_t *angles); //to be given numJoints amount of space
        uint32_t getJointVelocity(uint8_t port);
        void getJointVelocities(uint32_t *velocities); //to do
        void createJointMsgs(uint32_t *msgs);

    private:
        uint8_t numJoints;
        PCA9548A_MUX mux;
        bool muxFound = FALSE;
        uint8_t ports[MAX_JOINTS];
        AS5600 encoder;


        void findMux();
        void encoderScan();
        bool portsContains(uint8_t port);
        void addJointAngles(uint32_t *msgs);

        void addJointVelocities(uint32_t *msgs);
        void calculateVelocities(uint32_t *velocities);
        void calculateFloatVelocities(float *floatVelocities);
        void fillAnglesTimes(float floatAngles[][VELOCITY_SAMPLES], uint32_t *times);
        void wrapAngles(float floatAngles[][VELOCITY_SAMPLES]);
        float calculateGrad(float *floatAngles, uint32_t *times);


};

#endif