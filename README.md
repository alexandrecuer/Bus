# Bus

Single Wire Bus - a subclass of the OneWire library

OneWire library is maintained by Paul Stoffregen<br>
http://www.pjrc.com/teensy/td_libs_OneWire.html<br>
or<br>
https://github.com/PaulStoffregen/OneWire

Bus class scans the 1 wire Bus connected to an arduino UNO analog pin and stores the ROMs in an array<br>
1-wire components are commercialized by Maxim Integrated Products, Inc.<br>
https://www.maximintegrated.com/en/products/digital/one-wire.html

It has been tested with two configurations, corresponding to my needs :
- one DS2438
- one DS2438 and one DS18B20<br>
In all cases, an analog humidity sensor HIH4000 (Honeywell International Inc.) was connected to the DS2438 VAD input

Anyway it should work with more than 2 devices on the bus

Please note that you need to install the OneWire library first in your arduinono library directory

<b>Usage</b>

Bus bus (uint8_t pin)<br>
Create the Bus object, using a specific pin

bus.begin()<br>
Initialize the bus with a number of devices equal to zero

bus.void find()<br>
scan the bus and stores the ROMs in an array

bus.nb()<br>
return the number of devices on the bus

bus.ROM(byte j)<br>
print to serial the ROM number of the device number j

bus.is28(byte j)<br>
return true if device is from 28 family and false if not

bus.is26(byte j)<br>
return true if device is from 26 family and false if not
  
bus.get28temperature(byte j)<br>
return temperature from the device i which is a DS1820 sensor

  float get26temperature(byte j);
  //mode can be "vdd" or "vad"
  float get26voltage(byte j, char *mode);
  boolean read26PageZero(byte j, uint8_t *data);
  void write26PageZero(byte j, uint8_t *data);
