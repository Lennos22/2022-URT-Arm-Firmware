#include "JointMotor.h"
#include "BasicDefs.h"
#include <TMCStepper.h>
#include "./src/MotorAux/TMC5160PRO.h"

//Define motor driver parameters
#define RMS_CURRENT 500
#define MICROSTEPS 128

#define CSN_1 PB6
#define CSN_2 PB10
#define CSN_3 PA10

#define IHOLD           20
#define IRUN            20
#define IHOLDDELAY       6
#define TOFF             3
#define HSTRT            4
#define HEND             1
#define TBL              2
#define CHM              0
#define MOTOR_TPWMTHRS 500

#define MOTOR_A1      2000  
#define MOTOR_V1     50000 
#define MOTOR_AMAX    2500
#define MOTOR_DMAX     700
#define MOTOR_D1      1400
#define MOTOR_VSTOP     10

#define FULL_ROTATION 51200

#define R_SENSE 0.075f 



const int MOSI_PIN = PB15;          
const int MISO_PIN = PB14;
const int SCLK_PIN = PB13;


void JointMotor::begin(){
    if(VERBOSE){
        Serial.println("Starting joint motor module");
    }

    //Begin SPI bus to motors
    SPI.setMOSI(MOSI_PIN);
    SPI.setMISO(MISO_PIN);
    SPI.setSCLK(SCLK_PIN);
    SPI.begin();

    numMotors = 1;

    
    TMC5160Stepper driver1(CSN_1, R_SENSE);
    driver1.begin();

    Serial.print("DRV1_STATUS=0b"); 
    Serial.println(driver1.DRV_STATUS(), BIN);

    Serial.print("DRV1_IOIN=0b");
    Serial.println(driver1.IOIN(), BIN);

    Serial.print("DRV1_GCONF=0b");
    Serial.println(driver1.GCONF(), BIN);

    driver1.toff(3);                 // Enables driver in software
    driver1.hstrt(4);
    driver1.hend(1);
    driver1.tbl(2);
    driver1.chm(0);

    delay(100);
    driver1.rms_current(RMS_CURRENT);        // Set motor RMS current
    driver1.microsteps(MICROSTEPS);          // Set microsteps to 1/64th

    delay(100);
    driver1.ihold(15);
    driver1.irun(15);
    // driver1.GLOBAL_SCALER(200);
    driver1.iholddelay(6);

    delay(100);
    driver1.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
    driver1.TPWMTHRS(500);

    delay(30);
    driver1.a1(MOTOR_A1);
    delay(30);
    driver1.v1(MOTOR_V1);
    delay(30);
    driver1.AMAX(MOTOR_AMAX);
    delay(30);
    driver1.DMAX(MOTOR_DMAX);
    delay(30);
    driver1.d1(MOTOR_D1);
    delay(30);
    driver1.VSTOP(MOTOR_VSTOP);
    delay(30);



    delay(100);

    driver1.RAMPMODE(1);
    driver1.VMAX(10000);

}

void JointMotor::setSpeed(uint32_t newSpeed){
    if(newSpeed>10000000) newSpeed = 100;
    if(newSpeed<100) newSpeed = 100;
    speed = newSpeed;
}