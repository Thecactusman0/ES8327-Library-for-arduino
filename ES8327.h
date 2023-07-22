/* Arduino library for the ES8327 audio CODEC IC. Developed by Nicholas Gray, work started 27th July 2023
#####################################THIS LIBRARY IS MEANT FOR USE WITH THE ESP32####################################*/

#include "Arduino.h"
#include <Wire.h>

#ifndef ES8327_h
#define ES8327_h


class ES8327
{
  public: //for stuff that needs to be accessed outside the library
    ES8327(TwoWire& wire, int HPINT,int ADR);//i2C pins, headphone int pin, address High is 0x19 address and low is 0x18
    bool begin();
    void setVolumeOut(float vol);// Vol (dB) must be in the range of -95.5 to 32 in increments of 0.5
    void setVolumeIn(float vol);// Vol (dB) must be in the range of -95.5 to 32 in increments of 0.5
    void setWordLength(int length); // set i2s word length
  private: //for stuff that needs to be accessed only inside the library
    void writeRegister(int register_address, int val);
    int readRegister(int register_address);
    TwoWire& _wire;
    int _ADR; //i2c address
    int _HPINT; //headphone int pin
};

#endif