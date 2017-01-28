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

#include "PacketBuffer.h"

const char* PacketBuffer::buffer() { return _buf; }

byte PacketBuffer::length() { return _fill; }

void PacketBuffer::reset()
	{ 
    memset(_buf,0,sizeof(_buf));
    _fill = 0;
    }
        

void PacketBuffer::start(const char *server_basedir, const char *server_apikey, const int sensoring_node_id)
    {
    print("GET /");
    print(server_basedir);
    print("/input/post.json?");
    print("apikey="); print(server_apikey);
    print("&node=");  print(sensoring_node_id);
    print("&json={");
    }
 
void PacketBuffer::end(const IPAddress server_ip)
    {
    println("} HTTP/1.0");
    print("Host: ");
    println(server_ip);
    }

void PacketBuffer::printDataEMONformat(float data, const byte sensoring_base_ip, const int n, const char family[2], const char *lib)
    {
    print(lib);
    if(family)print(family);
    if(sensoring_base_ip){print("_");print(sensoring_base_ip);}
    print("_");print(n);
    print(":");print(data);
    }

void PacketBuffer::printDataLCDformat(float data, const int n, const char family[2], const char lib[2], const char unit[1])
    {
    // !! 16 characters maximum
    if(family)print(family);//2 characters
    print(" ");print(n);//2 characters
    print(" ");print(lib);//max 3 characters
    print(" ");print(data,1);//print data with 1 digit of precision > 5 or 6 characters including the blank
    print(unit);//1 character    
    }