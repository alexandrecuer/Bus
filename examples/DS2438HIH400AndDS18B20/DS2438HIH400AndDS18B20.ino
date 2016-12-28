//temperature and humidity monitoring
//example with one DS18B20 and one DS2438 on the bus
//an HIH4000 is attached on the DS2438 VAD input

#include "Bus.h"

const uint8_t ONE_WIRE_PIN = 2;
Bus bus(ONE_WIRE_PIN);

void setup() {
  Serial.begin(9600);
  bus.begin();
  bus.reset_search();
  bus.find();
}

void loop() {
  for(int i=0;i<bus.nb();i++){
    bus.ROM(i);
    if(bus.is28(i))
      {
      Serial.print(" : DS18B20 sensor : temperature in celsius : ");
      Serial.println(bus.get28temperature(i));
      }
    if(bus.is26(i))
      {
      Serial.print(" : DS2436 sensor : temperature in celsius : ");
      float celsius2438 = bus.get26temperature(i);
      float vdd = bus.get26voltage(i,"vdd");
      float vad = bus.get26voltage(i,"vad");
      float rh = (vad/vdd - 0.16)/0.0062;
      float truerh = rh/(1.0546-0.00216*celsius2438);
      Serial.print(celsius2438);
      Serial.print(" - RH in % : ");
      Serial.println(truerh);
      }
    }
  Serial.println();
  delay(5000);

}
