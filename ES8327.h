/* Arduino library for the ES8327 audio CODEC IC. Developed by Nicholas Gray, work started 27th July 2023
#####################################THIS LIBRARY IS MEANT FOR USE WITH THE ESP32####################################*/

#include "Arduino.h"
#include <Wire.h>

#ifndef ES8327_h
#define ES8327_h
#define Headphone 1
#define Internal 2


class ES8327
{
  public: //for stuff that needs to be accessed outside the library
    ES8327(TwoWire& wire, int HPINT,int ADR);//i2C pins, headphone int pin, address High is 0x19 address and low is 0x18
    bool begin();
    void setVolumeOut(float vol);// Vol (dB) must be in the range of -95.5 to 32 in increments of 0.5
    void setVolumeIn(float vol);// Vol (dB) must be in the range of -95.5 to 32 in increments of 0.5
    void setWordLength(int length); // set i2s word length
    void micAutoLevelControlOn(); //Turn on microphone automatic level control
    void micAutoLevelControlOff(); //Turn off microphone automatic level control
    void autoLevelControlTargetLevel(float target); //Target level for auto level control in dB, acceptable values are -30,-27,-24,-21,-19,-17,-15,-13.5,-12,-10.5,-9,-7.5,-6,-4.5,-3,-1.5 Default -30 if function not called
    void micSelect(int mic,int gain); //Accepts "Headphone" and "Internal" for mic and 0,3,6,9,12,15,18,21,24,27,30
  private: //for stuff that needs to be accessed only inside the library
    void writeRegister(int register_address, int val);
    int readRegister(int register_address);
    TwoWire& _wire;
    int _ADR; //i2c address
    int _HPINT; //headphone int pin
};

#endif