#include <ESP8266WiFi.h>                // Library of all types of ESP8266 modules.

#define WLAN_SSID       "RPSISHERE"     // Name of the wi-fi you want to connect 
#define WLAN_PASS       "underfire"         // Password of that wi-fi


void setup()
{
  Serial.begin(9600);
  delay(10);

  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)       // While wi-fi is still connecting 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP()); 

}

void loop() {
  // put your main code here, to run repeatedly:

}
