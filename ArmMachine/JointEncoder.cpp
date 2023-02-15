#include "JointEncoder.h"
#include "BasicDefs.h"

#include <Wire.h>


const float DEG_PER_RAW = 360.0/4096;

void JointEncoder::begin(){
    Wire.setClock(400000);
    Wire.begin();
    if(VERBOSE){
        Serial.println("Started joint encoder module");
    }
    numJoints = 0;

    findMux();
    if(muxFound){
        encoderScan();
    }
    if(numJoints>0) {
        mux.setPort(ports[0]);
        encoder.begin();
    }



    //Want to setup mux and check mux is working
    //Want to scan mux ports for AS5600 devices 
    //For each found devices store port number

    if(VERBOSE){
        Serial.print("Number of joints: ");
        Serial.println(numJoints);
    }
}

void JointEncoder::createJointMsgs(uint32_t *msgs){
    for(int i = 0; i<numJoints; i++){
        mux.setPort(ports[i]);
        if(encoder.isConnected()) msgs[i] = 1<<31;
    }

    addJointAngles(msgs);
    addJointVelocities(msgs);
}

void JointEncoder::findMux(){
    if(VERBOSE){
        Serial.println("Looking for I2C MUX");
    }
    Wire.setClock(400000);
    Wire.beginTransmission(MUX_ADDRESS);
    int result = Wire.endTransmission();
    if(result==0){
        if(VERBOSE){
            Serial.print("I2C mux found at address: 0x");
            Serial.println(MUX_ADDRESS,HEX);
        }
        mux.begin();
        muxFound = TRUE;
    } else {
        if(VERBOSE){
            Serial.println("No mux found");           
        }
    }
}

void JointEncoder::encoderScan(){
    for(int i = 0; i< MUX_PORTS; i++){
        mux.setPort(i);
        Wire.beginTransmission(0x36);       
        int result = Wire.endTransmission();
        if(result == 0){
            ports[numJoints++] = i;
        }
    }
    if(VERBOSE){
        Serial.print("Encoders found at ports:");
        for(int i = 0; i<numJoints; i++){
            Serial.print(" ");
            Serial.print(ports[i]);
        }
        Serial.println();
    }
}

uint32_t JointEncoder::getJointAngle(uint8_t port){
    uint32_t result = 0xffffffff;
    for(int i = 0; i<numJoints; i++){
        if(port==ports[i]) break;
        if(i==numJoints-1) return result;
    }
    mux.setPort(port);
    result = encoder.rawAngle();
    if(VERBOSE){
        Serial.print("Angle at port: ");
        Serial.print(port);
        Serial.print(" is: ");
        Serial.println(result);
    }
    return result;
}

/*Must be given enough space for all ports, i.e. numJoints*/
void JointEncoder::getJointAngles(uint32_t *angles){
    for(int i = 0; i<numJoints; i++) angles[i] = JointEncoder::getJointAngle(ports[i]);
}

bool JointEncoder::portsContains(uint8_t port){
    for(int i = 0; i<numJoints; i++){
        if(port==ports[i]) return TRUE;
        if(i==numJoints-1) return FALSE;
    }
}

void JointEncoder::addJointAngles(uint32_t *msgs){
    for(int i = 0; i<numJoints; i++){
        uint32_t tempAngle = getJointAngle(ports[i]);
        msgs[i] |= tempAngle;        
    }
}

void JointEncoder::addJointVelocities(uint32_t *msgs){
    uint32_t velocities[numJoints];
    for(int i = 0; i<numJoints; i++) velocities[i] = 0;
    calculateVelocities(velocities);
    for(int i = 0; i<numJoints; i++){
        msgs[i] |= (velocities[i]<<12);
    }
}

void JointEncoder::calculateVelocities(uint32_t *velocities){
    float floatVelocities[numJoints];
    for(int i = 0; i<numJoints; i++) floatVelocities[i] = 0.0;
    calculateFloatVelocities(floatVelocities);
    for(int i = 0; i<numJoints; i++) {
        float tempFloatVel = floatVelocities[i]>0?floatVelocities[i]:-floatVelocities[i];
        tempFloatVel = tempFloatVel>1080.0?1080.0:tempFloatVel;
        uint32_t tempVelocity = (uint32_t) (tempFloatVel*2047)/1080;
        if(floatVelocities[i]<0) tempVelocity |= (1<<11);
        velocities[i] = tempVelocity;
    }
}

void JointEncoder::calculateFloatVelocities(float *floatVelocities){
    float floatAngles[numJoints][VELOCITY_SAMPLES];
    uint32_t times[VELOCITY_SAMPLES];
    for(int i = 0; i<VELOCITY_SAMPLES; i++){
        times[i] = 0;
        for(int j = 0; j<numJoints; j++){
            floatAngles[j][i] = 0.0;
        }
    }

    fillAnglesTimes(floatAngles, times);
    for(int i = 0; i<numJoints; i++) {
        floatVelocities[i] = calculateGrad(floatAngles[i], times);
    }

}


void JointEncoder::fillAnglesTimes(float floatAngles[][VELOCITY_SAMPLES], uint32_t *times){
    for(int i = 0; i<VELOCITY_SAMPLES; i++){
        for(int j = 0; j<numJoints; j++){
            mux.setPort(ports[j]);
            floatAngles[j][i] = (float) DEG_PER_RAW * encoder.rawAngle();
        }
        times[i] = micros();
    }
    wrapAngles(floatAngles);
}


void JointEncoder::wrapAngles(float floatAngles[][VELOCITY_SAMPLES]){
    
    for(int i = 0; i<numJoints; i++){
        float min = 0; float max = 0;
        for(int j = 0; j<VELOCITY_SAMPLES; j++){
            min = floatAngles[i][j]<min?floatAngles[i][j]:min;
            max = floatAngles[i][j]>max?floatAngles[i][j]:max;
        }
        if(max-min>180.0){
            for(int j = 0; j<VELOCITY_SAMPLES; j++){
                floatAngles[i][j] = floatAngles[i][j]<180.0?floatAngles[i][j]+360.0:floatAngles[i][j];
            }
        }
    }

}


float JointEncoder::calculateGrad(float *floatAngles, uint32_t *times){
    float res = 1080.0;
    
    float sumX  = 0.0;
    float sumX2 = 0.0;
    float sumY  = 0.0;
    float sumXY = 0.0;

    for(int i = 0; i<numJoints; i++){
        float timeI = (times[i]-times[0])/1000000.0;
        sumX  += timeI;
        sumX2 += timeI*timeI;
        sumY  += floatAngles[i];
        sumXY += floatAngles[i]*timeI; 
    }

    
    res = ((numJoints*sumXY) - (sumX*sumY))/((numJoints*sumX2) - (sumX*sumX));
    return res;
}
