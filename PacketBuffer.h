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

#ifndef PacketBuffer_h
#define PacketBuffer_h

#include "Arduino.h"
#include "Ethernet.h"

class PacketBuffer : public Print {
public:
  PacketBuffer () : _fill (0) {};
  const char* buffer();
  byte length();
  void reset();
  void start(const char *server_basedir, const char *server_apikey, const int sensoring_node_id);
  void end(const IPAddress server_ip);
  void printDataEMONformat(float data, const byte sensoring_base_ip, const int n, const char family[2], const char *lib);
  void printDataLCDformat(float data, const int n, const char family[2], const char lib[2], const char unit[1]);
  virtual size_t write (uint8_t ch)
    { if (_fill < sizeof _buf) _buf[_fill++] = ch; }
	
private:
  byte _fill;
  char _buf[200];
};

#endif
