#include "Arduino.h"
#include "TMC5160PRO.h"
#include "SPI.h"

void printHeader(String title){
    int n = title.length();
    
    for(int i = 0; i<HEADER_LENGTH; i++) Serial.print("-");
    Serial.println("");

    for(int i = 0; i<(HEADER_LENGTH-n)/2; i++) Serial.print("-");

    Serial.print(title);

    for(int i = 0; i<(HEADER_LENGTH+1-n)/2; i++) Serial.print("-");
    Serial.println("");

    for(int i = 0; i<HEADER_LENGTH; i++) Serial.print("-");
    Serial.println("");

}

uint32_t TMC5160PRO::readReg(uint8_t address, uint8_t pin){
    uint8_t spi_status = 0;
    uint32_t res       = 0;

    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW); //reset chip select pins and set low

    SPI.beginTransaction(SPISettings(TMC5160PRO::spi_clock, MSBFIRST, SPI_MODE3));
    spi_status = SPI.transfer(address); //send address to read and get SPI status
    res = SPI.transfer(0x00); //read 4 bytes and shift left each time
    res << 8;
    res |= SPI.transfer(0x00);
    res << 8;
    res |= SPI.transfer(0x00);
    res << 8;
    res |= SPI.transfer(0x00);

    SPI.endTransaction(); //end spi

    digitalWrite(pin,HIGH); //end by setting chip select pin high

    return res;
}

void TMC5160PRO::writeReg(uint8_t address, uint8_t pin, uint32_t message){
    uint8_t spi_status = 0;
    
    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);

    address |= 0x80; //add leading 1 to make write command

    SPI.beginTransaction(SPISettings(TMC5160PRO::spi_clock, MSBFIRST, SPI_MODE3));
    spi_status = SPI.transfer(address);
    SPI.transfer(message>>16);
    SPI.transfer(message & 0xFFFF);
    SPI.endTransaction();

    digitalWrite(pin, HIGH);
}

void TMC5160PRO::writeChopConf(uint8_t pin){
    TMC5160PRO::writeReg(TMC5160PRO::CHOPCONF_REG,pin,0x000100C3);
}
void TMC5160PRO::writeIHOLD_IRUN(uint8_t pin){
    TMC5160PRO::writeReg(TMC5160PRO::IHOLD_IRUN_REG,pin,0x00061F0A);
}
void TMC5160PRO::writeTPOWERDOWN(uint8_t pin){
    TMC5160PRO::writeReg(TMC5160PRO::TPOWERDOWN_REG,pin,0x0000000A);
}
void TMC5160PRO::writeGCONF(uint8_t pin){
    TMC5160PRO::writeReg(TMC5160PRO::GCONF_REG,pin,0x00000004);
}

uint32_t TMC5160PRO::readDRV_STATUS(uint8_t pin){
    return TMC5160PRO::readReg(TMC5160PRO::DRV_STATUS_REG,pin);
}


TMC5160PRO::DRV_STATUS::DRV_STATUS(uint32_t num){
    TMC5160PRO::DRV_STATUS::num          = num;
    TMC5160PRO::DRV_STATUS::standStill   = standStillMask   & num;
    TMC5160PRO::DRV_STATUS::openLoadB    = openLoadBMask    & num;
    TMC5160PRO::DRV_STATUS::openLoadA    = openLoadAMask    & num;
    TMC5160PRO::DRV_STATUS::shortLoadB   = shortLoadBMask   & num;
    TMC5160PRO::DRV_STATUS::shortLoadA   = shortLoadAMask   & num;
    TMC5160PRO::DRV_STATUS::overTempPre  = overTempPreMask  & num;
    TMC5160PRO::DRV_STATUS::overTemp     = overTempMask     & num;
    TMC5160PRO::DRV_STATUS::motorStall   = motorStallMask   & num;
    TMC5160PRO::DRV_STATUS::csActual     = (csActualMask    & num) >> 16;
    TMC5160PRO::DRV_STATUS::fsActive     = fsActiveMask     & num;
    TMC5160PRO::DRV_STATUS::stealthChop  = stealthChopMask  & num;
    TMC5160PRO::DRV_STATUS::shortSupplyB = shortSupplyBMask & num;
    TMC5160PRO::DRV_STATUS::shortSupplyA = shortSupplyAMask & num;
    TMC5160PRO::DRV_STATUS::sgResult     = sgResultMask     & num;
}

void TMC5160PRO::DRV_STATUS::DRV_Status_print(){
            printHeader("Driver Status Report");
            Serial.print("Driver Status reading: 0b");
            Serial.println(num,BIN);
            if(standStill){
                Serial.println("Standstill: true");
            } else {
                Serial.println("Standstill: false");
            }

            if(openLoadB){
                Serial.println("Open load phase B: true");
            } else {
                Serial.println("Open load phase B: false");
            }

            if(openLoadA){
                Serial.println("Open load phase A: true");
            } else {
                Serial.println("Open load phase A: false");
            }

            if(shortLoadB){
                Serial.println("Short load phase B: true");
            } else {
                Serial.println("Short load phase B: false");
            }

            if(shortLoadA){
                Serial.println("Short load phase A: true");
            } else {
                Serial.println("Short load phase A: false");
            }

            if(overTempPre){
                Serial.println("Overtemperature pre-warning: true");
            } else {
                Serial.println("Overtemperature pre-warning: false");
            }

            if(overTemp){
                Serial.println("Overtemperature warning: true");
            } else {
                Serial.println("Overtemperature warning: false");
            }

            if(motorStall){
                Serial.println("Motor stall: true");
            } else {
                Serial.println("Motor stall: false");
            }

            Serial.print("Actual current (0-31): ");
            Serial.println(csActual,DEC);

            if(fsActive){
                Serial.println("Full step active: true");
            } else {
                Serial.println("Full step active: false");
            }

            if(stealthChop){
                Serial.println("Stealth Chop active: true");
            } else {
                Serial.println("Stealth Chop active: false");
            }

            if(shortSupplyB){
                Serial.println("Supply short phase B: true");
            } else {
                Serial.println("Supply short phase B: false");
            }

            if(shortSupplyA){
                Serial.println("Supply short phase A: true");
            } else {
                Serial.println("Supply short phase A: false");
            }

            Serial.print("Stall guard reading (0-1023): ");
            Serial.println(sgResult,DEC);

}

uint32_t TMC5160PRO::DRV_STATUS::to_num(){
    uint32_t res = 0;
    if(standStill)   res |= standStillMask;
    if(openLoadB)    res |= openLoadBMask;
    if(openLoadA)    res |= openLoadAMask;
    if(shortLoadB)   res |= shortLoadBMask;
    if(shortLoadA)   res |= shortLoadAMask;
    if(overTempPre)  res |= overTempPreMask;
    if(overTemp)     res |= overTempMask;
    if(motorStall)   res |= motorStallMask;
    if(fsActive)     res |= fsActiveMask;
    if(stealthChop)  res |= stealthChopMask;
    if(shortSupplyB) res |= shortSupplyBMask;
    if(shortSupplyA) res |= shortSupplyAMask;

    res |= (csActual<<16);
    res |= sgResult;
    

    return res;
}

TMC5160PRO::IOIN::IOIN(uint32_t num){
    TMC5160PRO::IOIN::num = num;
    TMC5160PRO::IOIN::reflStep     = reflStepMask     & num;
    TMC5160PRO::IOIN::refrDir      = refrDirMask      & num;
    TMC5160PRO::IOIN::encbDcenCfg4 = encbDcenCfg4Mask & num;
    TMC5160PRO::IOIN::encaDcenCfg5 = encaDcenCfg5Mask & num;
    TMC5160PRO::IOIN::drvEnn       = drvEnnMask       & num;
    TMC5160PRO::IOIN::encNDcoCfg6  = encNDcoCfg6Mask  & num;
    TMC5160PRO::IOIN::sdMode       = sdModeMask       & num;
    TMC5160PRO::IOIN::swcompIn     = swcompInMask     & num;
    TMC5160PRO::IOIN::version      = (versionMask     & num) >> 24;
}

void TMC5160PRO::IOIN::IOIN_print(){
    printHeader("IOIN Report");

    Serial.print("Driver IOIN reading: 0b");
    Serial.println(TMC5160PRO::IOIN::num,BIN);

    Serial.print("REFL STEP Pin: ");
    if(TMC5160PRO::IOIN::reflStep) Serial.println("High");
    else Serial.println("Low");

    Serial.print("REFR DIR Pin: ");
    if(TMC5160PRO::IOIN::refrDir) Serial.println("High");
    else Serial.println("Low");

    Serial.print("ENCB DCEN CFG4 Pin: ");
    if(TMC5160PRO::IOIN::encbDcenCfg4) Serial.println("High");
    else Serial.println("Low");

    Serial.print("ENCA DCIN CFG5 Pin: ");
    if(TMC5160PRO::IOIN::encaDcenCfg5) Serial.println("High");
    else Serial.println("Low");

    Serial.print("DRV ENN Pin: ");
    if(TMC5160PRO::IOIN::drvEnn) Serial.println("High");
    else Serial.println("Low");

    Serial.print("ENC N DC0 CFG6 Pin: ");
    if(TMC5160PRO::IOIN::encNDcoCfg6) Serial.println("High");
    else Serial.println("Low");

    Serial.print("SD Mode: ");
    if(TMC5160PRO::IOIN::sdMode) Serial.println("High");
    else Serial.println("Low");

    Serial.print("SWCOMP IN Pin: ");
    if(TMC5160PRO::IOIN::swcompIn) Serial.println("High");
    else Serial.println("Low");

    Serial.print("Version: 0x");
    Serial.println(TMC5160PRO::IOIN::version,HEX);
}


TMC5160PRO::GCONF::GCONF(uint32_t num){
    TMC5160PRO::GCONF::num = num;

    TMC5160PRO::GCONF::recalibrate          = num & recalibrateMask;
    TMC5160PRO::GCONF::fastSTandStill       = num & fastSTandStillMask;
    TMC5160PRO::GCONF::enPwmMode            = num & enPwmModeMask;
    TMC5160PRO::GCONF::multistepFilt        = num & multistepFiltMask;
    TMC5160PRO::GCONF::shaftDir             = num & shaftDirMask;
    TMC5160PRO::GCONF::diag0Err             = num & diag0ErrMask;
    TMC5160PRO::GCONF::diag0Otpw            = num & diag0OtpwMask;
    TMC5160PRO::GCONF::diag0Step            = num & diag0StepMask;
    TMC5160PRO::GCONF::diag1Dir             = num & diag1DirMask;
    TMC5160PRO::GCONF::diag1Index           = num & diag1IndexMask;
    TMC5160PRO::GCONF::diag1Onstate         = num & diag1OnstateMask;
    TMC5160PRO::GCONF::diag1StepsSkipped    = num & diag1StepsSkippedMask;
    TMC5160PRO::GCONF::diag0IntPushPull     = num & diag0IntPushPullMask;
    TMC5160PRO::GCONF::diag1PoscompPushpull = num & diag1PoscompPushpullMask;
    TMC5160PRO::GCONF::smallHysteresis      = num & smallHysteresisMask;
    TMC5160PRO::GCONF::stopEnable           = num & stopEnableMask;
    TMC5160PRO::GCONF::directMode           = num & directModeMask;
    TMC5160PRO::GCONF::testMode             = num & testModeMask;

}








