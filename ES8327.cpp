#include "Arduino.h"
#include "ES8327.h"


ES8327::ES8327(TwoWire& wire, int HPINT, int ADR) : _wire(wire), _HPINT(HPINT), _ADR(ADR)
{

}


bool ES8327::begin()
{
pinMode(_HPINT,INPUT);
//writeRegister(0x01,0x3C);
writeRegister(0x01,0XFF); //Turn on all clocks
writeRegister(0xF9,0x02);
writeRegister(0x02,0x00);
writeRegister(0x03,0x05);
writeRegister(0x04,0x01);//Clock Configuration，If your MCLK/LRCK is not equal to 256, please contact me to modify this section
writeRegister(0x05,0x00);
writeRegister(0x06,0x30);
writeRegister(0x07,0x2D);
writeRegister(0x08,0x26);
//writeRegister(0x09,0xFF);
writeRegister(0x0A,0x1F);
writeRegister(0x0B,0x1F);
writeRegister(0x0C,0x04);
writeRegister(0x10,0xC8);
writeRegister(0x11,0x88);
writeRegister(0x12,0x20);
writeRegister(0x14,0x00);
writeRegister(0x19,0xF0);
writeRegister(0x1D,0x08);
//writeRegister(0x25,0x22);
writeRegister(0x25,0XAA);
writeRegister(0x29,0x00);
writeRegister(0x2A,0x22);
writeRegister(0x2B,0x44);
writeRegister(0x2C,0xBF);
writeRegister(0x2D,0xBF);
writeRegister(0x2E,0x00);
writeRegister(0x4A,0x00);
writeRegister(0x4D,0x08);
writeRegister(0x4E,0x20);
//writeRegister(0x4F,0x15);
writeRegister(0x4F,0X01);
writeRegister(0x50,0xBF);
writeRegister(0x56,0x88);
writeRegister(0x57,0x10);
writeRegister(0x58,0x08);
writeRegister(0x59,0x45);
writeRegister(0x5A,0x90);
writeRegister(0x5B,0x00);
writeRegister(0x15,0x00);
//writeRegister(0x00,0x81);
writeRegister(0x00,0x80);//Powers state machine, Initalize in slave mode, Enable power up sequence, dont reset circuits
//writeRegister(0x24,0x0F);
writeRegister(0x24,0X01);
//writeRegister(0x16,0x17);
writeRegister(0x16,0x00);//Power on analog
//writeRegister(0x27,0x00);
writeRegister(0x27,0x77);


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

void ES8327::micSelect(int mic,int gain)
{
int i;
int x;
if(mic == 2){i = 32;};
if(mic == 1){i = 16;};
x = gain/3;
writeRegister(0x23,i+x);  
}

void ES8327::micAutoLevelControlOn()
{
writeRegister(0x32,0x08);  
}

void ES8327::micAutoLevelControlOff()
{
writeRegister(0x32,0x00);  
}

void ES8327::autoLevelControlTargetLevel(float target)
{
int i;

if(target == -30){i = 0X00;}
if(target == -27){i = 0X01;}
if(target == -24){i = 0X02;}
if(target == -21){i = 0X03;}
if(target == -19){i = 0X04;}
if(target == -17){i = 0X05;}
if(target == -15){i = 0X06;}
if(target == -13.5){i = 0X07;}
if(target == -12){i = 0X08;}
if(target == -10.5){i = 0X09;}
if(target == -9){i = 0X0A;}
if(target == -7.5){i = 0X0B;}
if(target == -6){i = 0X0C;}
if(target == -4.5){i = 0X0D;}
if(target == -3){i = 0X0E;}
if(target == -1.5){i = 0X0F;}

writeRegister(0x33,i);  
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
if(i<0){i=0;};
if(i>255){i=255;};
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
if(i<0){i=0;};
if(i>255){i=255;}; 
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