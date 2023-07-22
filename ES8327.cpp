#include "Arduino.h"
#include "ES8327.h"


ES8327::ES8327(TwoWire& wire, int HPINT, int ADR) : _wire(wire), _HPINT(HPINT), _ADR(ADR)
{
  // Constructor body
}


bool ES8327::begin()
{
pinMode(_HPINT,INPUT);
writeRegister(0x2C,0);//Mute ADC
writeRegister(0x50,0);//Mute DAC
writeRegister(0x00,0x80);//Powers state machine, Initalize in slave mode, Enable power up sequence, dont reset circuits
writeRegister(0x01,0XFF); //Turn on all clocks
writeRegister(0x4F,0X01);//Unmute ADC
writeRegister(0x16,0x00);//Power on analog
writeRegister(0x17,0xC0);//Power on adc

writeRegister(0x23,0x20);//Setup input mic on mic in 2

writeRegister(0x25,0XAA);//Enable output
writeRegister(0x27,0x77);//Enable output
writeRegister(0x2C,191);//Unmute ADC
writeRegister(0x50,191);//Unmute DAC

  for (int deviceAddress = 1; deviceAddress < 127; deviceAddress++) {
    _wire.beginTransmission(deviceAddress);
    int error = _wire.endTransmission();
    if (deviceAddress == _ADR) {
      if (error == 0) {
        return true;
      }
    }
  }

  return false;



 
}

void ES8327::setWordLength(int length) //works for 16, 18, 20, 24 and 32 bit
{
 int i;

 switch (length)
 {
 case 16:
 i = 0X0C; // 000 011 00 in decimal
 break;

 case 18:
 i = 0X08; // 000 010 00 in decimal
 break;

 case 20:
 i = 0X04; // 000 001 00 in decimal
 break;

 case 24:
 i = 0X00; // 000 000 00 in decimal
 break;

 case 32:
 i = 0X10; // 000 100 00 in decimal
 break;

 }

writeRegister(0x13,i);
Serial.print("Value read:");
Serial.println(readRegister(0X13),HEX);
}



void ES8327::setVolumeOut(float vol)//Sets DAC, Vol (dB) must be in the range of -95.5 to 32 in increments of 0.5
{
//0 = -95.5dB
//191 = 0dB
//255 = 32dB
int i;
int in_min = -95.5;
int in_max = 32;

i = (vol - in_min) * 255 / (in_max - in_min);
if(i<0){i=0};
if(i>255){i=255};
writeRegister(0x50,i);

}

void ES8327::setVolumeIn(float vol)// Sets ADC, Vol (dB) must be in the range of -95.5 to 32 in increments of 0.5
{
//0 = -95.5dB
//191 = 0dB
//255 = 32dB
int i;
int in_min = -95.5;
int in_max = 32;

i = (vol - in_min) * 255 / (in_max - in_min);
if(i<0){i=0};
if(i>255){i=255}; 
writeRegister(0x2C,i);

}


void ES8327::writeRegister(int register_address, int data)
{
_wire.beginTransmission(_ADR); //Mute ADC
_wire.write(register_address);
_wire.write(data);	
_wire.endTransmission();   
}

int ES8327::readRegister(int register_address) {
  _wire.beginTransmission(_ADR);
  _wire.write(register_address);
  _wire.endTransmission();

  _wire.requestFrom(_ADR, (int)1);
  if (_wire.available()) {
    return _wire.read();
  }

  return 0;  // Return default value if reading fails
}