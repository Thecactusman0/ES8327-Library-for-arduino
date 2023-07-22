#include "Audio.h"
#include "ES8327.h"

#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26
#define I2S_MCLK       3

#define buttonUpPin 13 //connect from io pin to ground
#define buttonDownPin 12

float i;
bool pressedAlready;
bool pressedAlready2;

ES8327 codec(Wire,2,0x18);//wire object,hp_int, address
Audio audio;

String ssid =     "*******";
String password = "*******";

void setup()
{

Wire.begin();
Serial.begin(115200);

pinMode(buttonUpPin, INPUT_PULLUP);  //Setup buttons
pinMode(buttonDownPin, INPUT_PULLUP); 

WiFi.disconnect();
WiFi.mode(WIFI_STA);
WiFi.begin(ssid.c_str(), password.c_str());
while (WiFi.status() != WL_CONNECTED) delay(1500);

 if (!codec.begin()) 
 {
    Serial.println("Failed to find Codec");
    while (1) 
    {
      delay(10); //stop code in its tracks
    }
 }
  Serial.println("Codec Found!");
  
  codec.setWordLength(16);
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  audio.i2s_mclk_pin_select(I2S_MCLK);
  audio.setVolume(5); // 0...21
  
  audio.connecttohost("https://github.com/schreibfaul1/ESP32-audioI2S/raw/master/additional_info/Testfiles/Olsen-Banden.mp3");        // mp3
}



void loop() 
{
audio.loop();
  if (digitalRead(buttonUpPin) == 0 && !pressedAlready) //if button is pressed and not already triggered event
   {
      pressedAlready = true; //Set variable so this event only runs once
      i++; //Increment variable up 
   }

   if (digitalRead(buttonDownPin) == 0 && !pressedAlready2) //if button is pressed and not already triggered event
   {
      pressedAlready2 = true; //Set variable so this event only runs once
      i--; //Increment variable down  
   }

  if (digitalRead(buttonUpPin) == 1) //Reset variable when button is released 
  {
  pressedAlready = false; 
  } 

  if (digitalRead(buttonDownPin) == 1) //Reset variable when button is released 
  {
  pressedAlready2 = false; 
  }  
  
  codec.setVolumeOut(i);
}


void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}