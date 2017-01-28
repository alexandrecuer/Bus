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

#include "Arduino.h"
#include "OneWire.h"
#include "Bus.h"

void Bus::begin(){
    _nb=0;
  }

int Bus::nb(){
    return _nb;
  }

//maybe obsolete
//print the ROM number j on the serial port
void Bus::ROM(byte j){
    byte i;
    for( i = 0; i < 8; i++){Serial.print(_addr[j][i], HEX);Serial.print(" ");}   
  }

//use the 6 first bytes of the ROM number j for macaddress generation
void Bus::SROM(byte j, uint8_t *addr){
    for( byte i = 0; i < 6; i++)addr[i]=_addr[j][i];
  }
//convert the ROM to char
//we want to print all zero even non significative ones
//permit use of the print function via Serial or LCD
char* Bus::ROMtochar(byte j){
    //char chaine[16];
	char *chaine = (char *)malloc(16*sizeof(char)); // allocate memory from the heap
    int sz = sprintf(chaine,"%02x%02x%02x%02x%02x%02x%02x%02x",_addr[j][0],_addr[j][1],_addr[j][2],_addr[j][3],_addr[j][4],_addr[j][5],_addr[j][6],_addr[j][7]);
	return chaine;
} 
bool Bus::is28(byte j){
    return _addr[j][0]==0x28;
  }

bool Bus::is26(byte j){
    return _addr[j][0]==0x26;
  }

void Bus::find(){
    byte addr[8];
    byte i;
    while(search(addr)){for( i = 0; i < 8; i++)_addr[_nb][i]=addr[i];_nb++; }
  }

float Bus::get28temperature(byte j){
    byte i;
    byte data[9];
    reset();
    select(_addr[j]);
    //temperature conversion
    write(0x44, 1);
    delay(1000);
    reset();
    select(_addr[j]);
    //read Scratchpad
    write(0xBE);
    //we read 9 bytes - byte 8 is a CRC error code
    for ( i = 0; i < 9; i++)data[i] = read();
    /*
    for ( i = 0; i < 9; i++) {
      Serial.print(data[i], HEX); 
      Serial.print(" ");
    }
    Serial.print(" CRC="); 
    Serial.print(crc8(data, 8), HEX);
    Serial.println();
    */
    int16_t raw = (data[1] << 8) | data[0];
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
    if(crc8(data, 8) == data[8]) return (float)raw / 16.0;
    else return 10000000000;
  }

float Bus::get26temperature(byte j){
    uint8_t data[9]; 
    reset();
    select(_addr[j]);
    //temperature conversion 
    write(0x44, 0);
    //temperature delay 
    delay(10);
    if (read26PageZero(j,data)) return (double)(((((int16_t)data[2]) << 8) | (data[1] & 0x0ff)) >> 3) * 0.03125;
    else return 10000000000;
  }

float Bus::get26voltage(byte j, const char mode[3]){
    uint8_t data[9];
    
    read26PageZero(j,data);
    
    //data[0] | 0x08 > bit 5 set to 1 in byte 0 page 0 > VDD selection 
    //data[0] & 0xF7 > bit 5 set to 0 in byte 0 page 0 > VAD selection
    if(mode=="vdd")data[0] = data[0] | 0x08;
    if(mode=="vad")data[0] = data[0] & 0xF7;
    
    /*
    for(int k=0;k<9;k++){Serial.print(data[k],HEX);Serial.print(" ");}
    Serial.println();
    */
    write26PageZero(j,data);
    
    reset();
    select(_addr[j]);
    //voltage conversion
    write(0xb4, 0);
    //voltage delay
    delay(8);
    
    if(read26PageZero(j,data))
      {
        /*
        for(int k=0;k<9;k++){Serial.print(data[k],HEX);Serial.print(" ");}
        Serial.println();
        */
        return (((data[4] << 8) & 0x00300) | (data[3] & 0x0ff)) / 100.0;
      }
    else return 10000000000;
  }

boolean Bus::read26PageZero(byte j, uint8_t *data) {
    reset();
    select(_addr[j]);
    //recall memory command
    write(0xb8, 0);
    write(0x00, 0);
    reset();
    select(_addr[j]);
   //read scratchpad 
    write(0xbe, 0);
    write(0x00, 0);
    //we read 9 bytes - byte 8 is a CRC error code
    for (int i = 0; i < 9; i++)data[i] = read();
    return crc8(data, 8) == data[8];
  }

void Bus::write26PageZero(byte j, uint8_t *data) {
    reset();
    select(_addr[j]);
    //write scratchpad 
    write(0x4e, 0);
    write(0x00, 0);
    for (int i = 0; i < 8; i++)write(data[i], 0);
    reset();
    select(_addr[j]);
    //copy scratchpad 
    write(0x48, 0);
    write(0x00, 0);
}