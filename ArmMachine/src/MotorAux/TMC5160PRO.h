#ifndef TMC5160PRO_h
#define TMC5160PRO_h
#include "Arduino.h"
#include "SPI.h"

#define HEADER_LENGTH 60

void printHeader(String title);

class TMC5160PRO{
    public:
        class DRV_STATUS{
            public:
                DRV_STATUS(uint32_t num);
                void DRV_Status_print();
                uint32_t to_num();
            private:
                uint32_t num;
                boolean  standStill;
                boolean  openLoadB;
                boolean  openLoadA;
                boolean  shortLoadB;
                boolean  shortLoadA;
                boolean  overTempPre;
                boolean  overTemp;
                boolean  motorStall;
                uint8_t  csActual;
                boolean  fsActive;
                boolean  stealthChop;
                boolean  shortSupplyB;
                boolean  shortSupplyA;
                uint16_t sgResult;

                static const uint32_t standStillMask   = 0x80000000;
                static const uint32_t openLoadBMask    = 0x40000000;
                static const uint32_t openLoadAMask    = 0x20000000;
                static const uint32_t shortLoadBMask   = 0x10000000;
                static const uint32_t shortLoadAMask   = 0x08000000;
                static const uint32_t overTempPreMask  = 0x04000000;
                static const uint32_t overTempMask     = 0x02000000;
                static const uint32_t motorStallMask   = 0x01000000;
                static const uint32_t csActualMask     = 0x001F0000;
                static const uint32_t fsActiveMask     = 0x00008000;
                static const uint32_t stealthChopMask  = 0x00004000;
                static const uint32_t shortSupplyBMask = 0x00002000;
                static const uint32_t shortSupplyAMask = 0x00001000;
                static const uint32_t sgResultMask     = 0x000003FF;
        };
        class IOIN{
            private:
                uint32_t num;
                boolean reflStep;
                boolean refrDir;
                boolean encbDcenCfg4;
                boolean encaDcenCfg5;
                boolean drvEnn;
                boolean encNDcoCfg6;
                boolean sdMode;
                boolean swcompIn;
                uint8_t version;

                static const uint32_t reflStepMask      = 0x00000001;
                static const uint32_t refrDirMask       = 0x00000002;
                static const uint32_t encbDcenCfg4Mask  = 0x00000004;
                static const uint32_t encaDcenCfg5Mask  = 0x00000008;
                static const uint32_t drvEnnMask        = 0x00000010;
                static const uint32_t encNDcoCfg6Mask   = 0x00000020;
                static const uint32_t sdModeMask        = 0x00000040;
                static const uint32_t swcompInMask      = 0x00000080;
                static const uint32_t versionMask       = 0xFF000000;
                
            public:
                IOIN(uint32_t num);
                void IOIN_print();
        };
        class GCONF{
            private:
                uint32_t num;
                boolean recalibrate;
                boolean fastSTandStill;
                boolean enPwmMode;
                boolean multistepFilt;
                boolean shaftDir;
                boolean diag0Err;
                boolean diag0Otpw;
                boolean diag0Step;
                boolean diag1Dir;
                boolean diag1Index;
                boolean diag1Onstate;
                boolean diag1StepsSkipped;
                boolean diag0IntPushPull;
                boolean diag1PoscompPushpull;
                boolean smallHysteresis;
                boolean stopEnable;
                boolean directMode;
                boolean testMode;

                static const uint32_t recalibrateMask          = 0x00000001;
                static const uint32_t fastSTandStillMask       = 0x00000002;
                static const uint32_t enPwmModeMask            = 0x00000004;
                static const uint32_t multistepFiltMask        = 0x00000008;
                static const uint32_t shaftDirMask             = 0x00000010;
                static const uint32_t diag0ErrMask             = 0x00000020;
                static const uint32_t diag0OtpwMask            = 0x00000040;
                static const uint32_t diag0StepMask            = 0x00000080;
                static const uint32_t diag1DirMask             = 0x00000100;
                static const uint32_t diag1IndexMask           = 0x00000200;
                static const uint32_t diag1OnstateMask         = 0x00000400;
                static const uint32_t diag1StepsSkippedMask    = 0x00000800;
                static const uint32_t diag0IntPushPullMask     = 0x00001000;
                static const uint32_t diag1PoscompPushpullMask = 0x00002000;
                static const uint32_t smallHysteresisMask      = 0x00004000;
                static const uint32_t stopEnableMask           = 0x00008000;
                static const uint32_t directModeMask           = 0x00010000;
                static const uint32_t testModeMask             = 0x00020000;

            public:
                GCONF(uint32_t num);


        };

        static void writeChopConf(uint8_t pin);
        static void writeIHOLD_IRUN(uint8_t pin);
        static void writeTPOWERDOWN(uint8_t pin);
        static void writeGCONF(uint8_t pin);

        static uint32_t readDRV_STATUS(uint8_t pin);

    private:
        static const uint32_t spi_clock = 2000000;

        /*SPI Register Addresses*/        
        //General configuration registers (p32-36)
        static const uint8_t GCONF_REG      = 0x00;

        //Velocity Dependent Driver Feature Control Registers (p38-39)
        static const uint8_t IHOLD_IRUN_REG = 0x10; 
        static const uint8_t TPOWERDOWN_REG = 0x11;
        static const uint8_t TPWMTHRS_REG   = 0x13;

        //Ramp generator motion control registers (p40-41)
        static const uint8_t RAMPMODE_REG   = 0x20;
        static const uint8_t XACTUAL_REG    = 0x21;
        static const uint8_t VACTUAL_REG    = 0x22;
        static const uint8_t VSTART_REG     = 0x23;
        static const uint8_t A1_REG         = 0x24;
        static const uint8_t V1_REG         = 0x25;
        static const uint8_t AMAX_REG       = 0x26;
        static const uint8_t VMAX_REG       = 0x27;
        static const uint8_t DMAX_REG       = 0x28;
        static const uint8_t D1_REG         = 0x2A;
        static const uint8_t VSTOP_REG      = 0x2B;
        static const uint8_t TZEROWAIT_REG  = 0x2C;
        static const uint8_t XTARGET_REG    = 0x2D;


        static const uint8_t CHOPCONF_REG   = 0x6C;
        static const uint8_t DRV_STATUS_REG = 0x6F;


        static uint32_t readReg(uint8_t address, uint8_t pin);
        static void writeReg(uint8_t address, uint8_t pin, uint32_t message);
};

#endif //TMC5160PRO_H
