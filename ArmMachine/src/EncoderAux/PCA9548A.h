#ifndef PCA9548A_h
#define PCA9548A_h

#include "Arduino.h"
#include <Wire.h>

class PCA9548A_MUX{
    public:
        const static uint8_t addr = 0x70;
        
        bool begin(uint8_t deviceAddress = addr, TwoWire &wirePort = Wire); //Check communication and initialize device
        bool isConnected();                                                                      //Returns true if device acks at the I2C address
        bool setPort(uint8_t portNumber);                                                        //Enable a single port. All other ports disabled.
        bool setPortState(uint8_t portBits);                                                     //Overwrite port register with all 8 bits. Allows multiple bit writing in one call.
        uint8_t getPort();                                                                       //Returns the bit position of the first enabled port. Useful for IDing which port number is enabled.
        uint8_t getPortState();                                                                  //Returns current 8-bit wide state. May have multiple bits set in 8-bit field.

private:
  TwoWire *_i2cPort;                                  //This stores the user's requested i2c port
  uint8_t _deviceAddress = addr; //Default unshifted 7-bit address
};

#endif