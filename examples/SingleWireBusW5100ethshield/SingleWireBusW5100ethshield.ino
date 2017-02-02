//temperature and humidity monitoring
//example with one DS18B20 and one DS2438 on the bus
//an HIH4000 is attached on the DS2438 VAD input
//W5100 ethernet shield attached to pins (9),10,11,12,13

#include "Bus.h"
#include <SPI.h>
#include <Ethernet.h>

long lastConnectionTime = 0;
boolean lastConnected = false;
const int postingInterval = 10000;

PacketBuffer str;

//initilize single wire bus
const uint8_t ONE_WIRE_PIN = 2;
Bus bus(ONE_WIRE_PIN);
int nbwire;

//object parameters
byte mac[6];
IPAddress ip(192,168,1,6);

//emon server parameters IP, basedir, apikey
IPAddress emonip(192,168,1,27);
char emonbasedir[] = "emoncms";
char emonapikey[] = "your_32_chars_api_key";
int node_id = 0;

EthernetClient client;


void setup() {
  Serial.begin(9600);
  
  bus.begin();
  bus.reset_search();
  bus.find();
  nbwire=bus.nb();

  Serial.println("V. 27/01/2017");
  Serial.print(nbwire);Serial.println(" 1wire dev.");
 
  for (byte i=0;i<nbwire;i++){
    char *c=bus.ROMtochar(i);
    Serial.println(c);
    free(c);
    delay(1000);
  }
  bus.SROM(0,mac);
  Ethernet.begin(mac, ip);
  Serial.print("sensoring base is at ");
  Serial.println(Ethernet.localIP());
  // give the ethernet module time to boot up:
  delay(1000);
}

void loop() {
  if(nbwire)
  {
  if(!client.connected() && lastConnected){Serial.println("disconnnecting");client.stop();}
  //in case millis has returned to 0 between two consecutive connections 
  //a return to zero of millis() which can occur after 50 days 
  if(millis()-lastConnectionTime<0)lastConnectionTime =0;
  if(!client.connected() && (millis()-lastConnectionTime > postingInterval))
    {
      if(client.connect(emonip,80))
        {
        str.reset();
        str.start(emonbasedir, emonapikey, node_id);
        for(int i=0;i<nbwire;i++){
          //bus.ROM(i) maybe faster....
          //bus.ROM(i);
          char *c=bus.ROMtochar(i);
          Serial.println(c);
          free(c);
          
          if(bus.is28(i))
            {
            float celsius18B20 = bus.get28temperature(i);
            //data formatting for emoncms
            str.printDataEMONformat(celsius18B20,ip[3],i,"28","T");
            if(i<nbwire-1)str.print(",");
            }
          if(bus.is26(i))
            {
            float celsius2438 = bus.get26temperature(i);
            float vdd = bus.get26voltage(i,"vdd");
            float vad = bus.get26voltage(i,"vad");
            float rh = (vad/vdd - 0.16)/0.0062;
            float truerh = rh/(1.0546-0.00216*celsius2438);
            
            //data formatting for emoncms
            str.printDataEMONformat(celsius2438,ip[3],i,"26","T");
            str.print(",");
            str.printDataEMONformat(truerh,ip[3],i,"26","H");
            if(i<nbwire-1)str.print(",");
            }  
          }
        str.end(emonip);
        Serial.println(str.buffer());
        client.println(str.buffer());
        Serial.println(str.length());
        Serial.println("disconnecting");
        client.stop();
        lastConnectionTime = millis();
        Serial.println(lastConnectionTime);
        }
        else Serial.println("connection failed");
      lastConnected = client.connected();
      Serial.println();  
      }
    }
  }
