#include "BluetoothA2DPSink.h"
#include "ES8327.h"

BluetoothA2DPSink a2dp_sink;
ES8327 codec(Wire,2,0x18);//wire object,hp_int, address

void setup() {
  Wire.begin();
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

  i2s_pin_config_t my_pin_config = {
        .bck_io_num = 27,
        .ws_io_num = 26,
        .data_out_num = 25,
        .data_in_num = 33
    };

  a2dp_sink.i2s_mclk_pin_select (3);
  a2dp_sink.set_pin_config(my_pin_config);
  a2dp_sink.start("MyMusic");  

}


void loop() {
  delay(1000); // do nothing
}