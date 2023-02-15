#include "ArmMachine.h"
#include "BasicDefs.h"


void ArmMachine::begin(){
    delay(100);
    state = 0x01;
    talker.begin();
    listener.begin();
    joints.begin();
    motors.begin();
}

void ArmMachine::run(){
    if(timeGuard()){
        state = 0x01;
        listener.readMessage();
        uint32_t nums[WORDS_PER_MESSAGE];
        for(int i = 0; i<WORDS_PER_MESSAGE; i++) nums[i] = 0;
        joints.createJointMsgs(&nums[1]);
        talker.sendMsg(nums, WORDS_PER_MESSAGE);
    }

}

bool ArmMachine::timeGuard(){
    uint32_t currentTime = micros();
    if((currentTime - lastRun) > RUN_PERIOD_MICROS){
        if(VERBOSE){
            Serial.print("Time (micros) since last run: ");
            Serial.println(currentTime - lastRun);
        }
        lastRun = currentTime;
        return TRUE;
    }
    return FALSE;
}