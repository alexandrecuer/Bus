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

Please note that you need to install the OneWire library first in your arduino library directory

You can find various PCB designs in the PCB subdirectory in order to construct your own single wire Bus

# Usage

<b>Bus bus (uint8_t pin)</b><br>
Create the Bus object, using a specific pin

<b>bus.begin()</b><br>
Initialize the bus with a number of devices equal to zero

<b>bus.void find()</b><br>
scan the bus and stores the ROMs in an array

<b>bus.nb()</b><br>
return the number of devices on the bus

<b>bus.ROM(byte j)</b><br>
print to serial the ROM number of the device number j

<b>bus.is28(byte j)</b><br>
return true if device is from 28 family and false if not

<b>bus.is26(byte j)</b><br>
return true if device is from 26 family and false if not
  
<b>bus.get28temperature(byte j)</b><br>
return temperature from the device j which is assumed to be a DS1820 sensor

<b>bus.get26temperature(byte j)</b><br>
return temperature from the device j which is assumed to be a DS2438 sensor

<b>bus.get26voltage(byte j, char *mode)</b><br>
return the vdd of vad voltage on the device j which is assumed to be a DS2438 sensor<br>
mode can be "vdd" or "vad"

<b>bus.read26PageZero(byte j, uint8_t *data)</b><br>
read page zero on the device j which is assumed to be a DS2438 sensor, and stores the result in data<br>
could be private only - used by get26voltage and get26temperature
  
<b>bus.write26PageZero(byte j, uint8_t *data)</b><br>
write page zero on the device j which is assumed to be a DS2438 sensor<br>
could be private only - used by get26voltage
