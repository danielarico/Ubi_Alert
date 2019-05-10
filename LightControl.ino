/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
 * Define Constants
 ****************************************/
#define TOKEN "***" // Your Ubidots TOKEN
#define WIFINAME "***" //Your SSID
#define WIFIPASS "***" // Your Wifi Pass
#define DEVICE_LABEL "***"  // Put here your Ubidots device label
#define VARIABLE_LABEL "***"  // Put here your Ubidots variable label 

Ubidots client(TOKEN);

byte relayOn[] = {0xA0, 0x01, 0x01, 0xA2}; //0xA00101A2
byte relayOff[] = {0xA0, 0x01, 0x00, 0xA1}; //0xA00100A1

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) 
{
  if ((char)payload[0]=='1') // Relay on
  { 
    Serial.write(relayOn, sizeof(relayOn));
    delay(10000);
    Serial.write(relayOff, sizeof(relayOff));
  }
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  client.ubidotsSetBroker("business.api.ubidots.com"); // Sets the broker properly for the business account
  client.setDebug(false); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE_LABEL); //Insert the dataSource and Variable's Labels
  Serial.begin(9600);
  }

void loop()
{
  if(!client.connected())
  {
    client.reconnect();
    client.ubidotsSubscribe(DEVICE_LABEL, VARIABLE_LABEL); //Insert the dataSource and Variable's Labels
  }
  client.loop();
}