#include <SoftwareSerial.h>
#include "ESP8266.h"
// We'll use a software serial interface to connect to ESP8266
SoftwareSerial esp8266(2,3);

void setup() 
{
  pinMode(13,OUTPUT);
  Serial.begin(9600);
  esp8266.begin(115200);
    
  esp8266.write("AT+CWMODE=2\r\n");                           // To set mode to 
  delay(3000);
  
  esp8266.write("AT+CWSAP=\"RPSISHERE\",\"hereisspr\",1,0\r\n");
  delay(3000);
  
  esp8266.write("AT+CIPMUX=1\r\n");
  delay(3000);
  
  esp8266.write("AT+CIPSERVER=1,1110\r\n");
  delay(3000);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(esp8266.available()) 
  {
    Serial.write(esp8266.read());
    char c = esp8266.read();
    Serial.write(c);
    if(c == 'B')
    {
      digitalWrite(13,HIGH);
    }
    else if(c == 'b')
    {
      digitalWrite(13,LOW);
    }
    
  }
  if (Serial.available()) 
  {
    esp8266.write(Serial.read());
  }
}
