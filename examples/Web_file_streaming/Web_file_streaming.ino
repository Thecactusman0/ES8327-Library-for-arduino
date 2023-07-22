#include "Audio.h"
#include "ES8327.h"

#define I2S_DOUT      25
#define I2S_BCLK      27
#define I2S_LRC       26
#define I2S_MCLK       3

ES8327 codec(Wire,2,0x18);//wire object,hp_int, address
Audio audio;

String ssid =     "*******";
String password = "*******";

void setup()
{

Wire.begin();
Serial.begin(115200);

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