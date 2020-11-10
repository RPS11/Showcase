//#include <Adafruit_Sensor.h>


#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"

#define Relay1            D1
#define Relay2            D2
#define Relay3            D3
#define Relay4            D4

#define DHTPIN            D6

#define LED               D5

#define WLAN_SSID       "RPSISHERE"
#define WLAN_PASS       "underfire"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "RPS11"
#define AIO_KEY         "00aaa13dd43e40abbf63459164b92e9f"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");

Adafruit_MQTT_Subscribe Light = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay1");
Adafruit_MQTT_Subscribe Fan = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay2");
Adafruit_MQTT_Subscribe NightLamp = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay3");
Adafruit_MQTT_Subscribe Plug = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay4");

#define DHTTYPE           DHT11     // DHT 11 

DHT dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

void MQTT_connect();

void setup() 
{
  Serial.begin(9600);

  delay(10);

  pinMode(LED, OUTPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);


  Serial.println(("::::: HeY Dharm What's uP :::::"));

  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  
    dht.begin();

  mqtt.subscribe(&Light);
  mqtt.subscribe(&Fan);
  mqtt.subscribe(&NightLamp);
  mqtt.subscribe(&Plug);
}

uint32_t x = 0;

void loop() {
  MQTT_connect();
  
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Light) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light.lastread);
      int Light_State = atoi((char *)Light.lastread);
      digitalWrite(Relay1, Light_State);
      
    }
    if (subscription == &Fan) {
      Serial.print(F("Got: "));
      Serial.println((char *)Fan.lastread);
      int Fan_State = atoi((char *)Fan.lastread);
      digitalWrite(Relay2, Fan_State);
    }
    if (subscription == &NightLamp) {
      Serial.print(F("Got: "));
      Serial.println((char *)NightLamp.lastread);
      int NightLamp_State = atoi((char *)NightLamp.lastread);
      digitalWrite(Relay3, NightLamp_State);
    }
    if (subscription == &Plug) {
      Serial.print(F("Got: "));
      Serial.println((char *)Plug.lastread);
      int Plug_State = atoi((char *)Plug.lastread);
      digitalWrite(Relay4, Plug_State);
      
    }
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  if (! Humidity.publish(h)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  if (! Temperature.publish(t)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  
}

void MQTT_connect() 
{
  int8_t ret;

  if (mqtt.connected()) 
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  digitalWrite(LED, HIGH);
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
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) 
    {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  digitalWrite(LED, HIGH);
  delay(2000);
  digitalWrite(LED, LOW);
}
