/*
Copyright (c) 2016, Alexandre CUER (alexandre.cuer@wanadoo.fr)

	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//--------------------------------------------------------------------------
*/

#ifndef Bus_h
#define Bus_h

#include "Arduino.h"
#include "OneWire.h"

class Bus : public OneWire {
public:
  Bus(uint8_t pin) : OneWire(pin) {};
  void begin();
  int nb();
  void ROM(byte j);
  bool is28(byte j);
  bool is26(byte j);
  void find();
  float get28temperature(byte j);
  float get26temperature(byte j);
  //mode can be "vdd" or "vad"
  float get26voltage(byte j, char *mode);
  boolean read26PageZero(byte j, uint8_t *data);
  void write26PageZero(byte j, uint8_t *data);
private:
  byte _nb;
  byte _addr[][8];
};

#endif
