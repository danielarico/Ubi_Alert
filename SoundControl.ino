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
#define RED_SIREN "***"  // Put here your Ubidots variable label
#define GREEN_SIREN "***"  // Put here your Ubidots variable label 

#define RESET D5
#define NEXT D6
#define PLAY D7

Ubidots client(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/
void activate(int pin)
{
  digitalWrite(pin, LOW);
  delay(500);
  digitalWrite(pin, HIGH);
}

void callback(char* topic, byte* payload, unsigned int length) 
{
  char val = payload[0];
  //Serial.println(topic);

  if (strstr(topic,"new-customer") && val == '1')
  {
    activate(RESET);
    activate(PLAY);
    delay(2000);
  }
  else if (strstr(topic,"churn") && val == '1')
  {
    activate(RESET);
    activate(NEXT);
    delay(2000);
  }
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {

  pinMode(RESET, OUTPUT);  
  pinMode(NEXT, OUTPUT);
  pinMode(PLAY, OUTPUT);

  digitalWrite(RESET, HIGH);
  digitalWrite(NEXT, HIGH);
  digitalWrite(PLAY, HIGH);
  
  // put your setup code here, to run once:
  client.ubidotsSetBroker("industrial.api.ubidots.com"); // Sets the broker properly for the business account
  client.setDebug(false); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  client.ubidotsSubscribe(DEVICE_LABEL, GREEN_SIREN); //Insert the dataSource and Variable's Labels
  client.ubidotsSubscribe(DEVICE_LABEL, RED_SIREN);
  Serial.begin(9600);
  }

void loop()
{
  if(!client.connected())
  {
    client.reconnect();
    client.ubidotsSubscribe(DEVICE_LABEL, GREEN_SIREN); //Insert the dataSource and Variable's Labels
    client.ubidotsSubscribe(DEVICE_LABEL, RED_SIREN);
  }
  client.loop();
}