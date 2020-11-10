#include <ESP8266WiFi.h>                // Library of all types of ESP8266 modules. 
#include <Adafruit_MQTT.h>              // Library of MQTT connection protocols of Adafruit 
#include "Adafruit_MQTT_Client.h"       // MQTT related functions

#define Relay1            D1            // Pin D1 of NodeMCU named as Relay1
#define Relay2            D2            // Pin D2 of NodeMCU named as Relay2
#define LED               LED_BUILTIN   // NodeMCU named as LED

#define WLAN_SSID       "RPSISHERE"     // Name of the wi-fi you want to connect 
#define WLAN_PASS       "underfire"         // Password of that wi-fi

#define AIO_SERVER      "io.adafruit.com"       //Server of our Dashboard (MQTT Broker)
#define AIO_SERVERPORT  1883                    // use 8883 for SSL
#define AIO_USERNAME    "RPS11"                 // Adafruit Dashboard Username
#define AIO_KEY         "00aaa13dd43e40abbf63459164b92e9f"                   // Adafruit Dashboard Key

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);    

Adafruit_MQTT_Subscribe Light = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay1");
Adafruit_MQTT_Subscribe Fan = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay2");
void MQTT_connect();

void setup()
{
  Serial.begin(9600);                 // Begins The Setup at a BAUD RATE of 9600 

  delay(10);

  pinMode(LED, OUTPUT);                  // Defines D5 Pin as OUTPUT pin  
  pinMode(Relay1, OUTPUT);               // Defines D1 Pin as OUTPUT pin
  pinMode(Relay2, OUTPUT);               // Defines D2 Pin as OUTPUT pin

  Serial.println("::::: Hey gUYZ What's uP :::::");
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);         //Begins trying to connect to WiFi
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
  
  mqtt.subscribe(&Light);           //Defines feed 1 
  mqtt.subscribe(&Fan);             // Defines feed 2
}

uint32_t x = 0;                     //

void loop() 
{
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) 
  {
     if (subscription == &Light)
     {
      Serial.print(F("Got: "));
      Serial.println((char *)Light.lastread);
      int Light_State = atoi((char *)Light.lastread);  // atoi:CONVERTS DATA RECIEVED TO INTEGER DATA TYPE
      digitalWrite(Relay1, Light_State);
      
     }
    if (subscription == &Fan) 
    {
      Serial.print(F("Got: "));
      Serial.println((char *)Fan.lastread);
      int Fan_State = atoi((char *)Fan.lastread);
      digitalWrite(Relay2, Fan_State);
    }
         
    }
}

void MQTT_connect()             //Function to connect to MQTT  
{
  int8_t ret;

  if (mqtt.connected())
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  digitalWrite(LED, HIGH);        // LED blinks twice to let us know that we are connecting to MQTT
  delay(200);
  digitalWrite(LED, LOW);
  delay(200);
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);
  delay(200);
  while ((ret = mqtt.connect()) != 0)
  { 
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);          // wait 5 seconds
    retries--;
    if (retries == 0) 
    {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  digitalWrite(LED, HIGH);              //LED blinks once to let us know MQTT is connected   
  delay(2000);
  digitalWrite(LED, LOW);
}
