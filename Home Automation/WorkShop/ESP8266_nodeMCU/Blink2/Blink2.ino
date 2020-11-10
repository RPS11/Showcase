void setup() 
{
  pinMode(2, OUTPUT);               // Initialize the 2(builtin led) pin as an output
  pinMode(5,OUTPUT);                // External LED
}
 
 void loop() 
{
  digitalWrite(2, LOW);             // Turn the LED on (Note that LOW is the voltage level
  digitalWrite(5, LOW);             
                                    // but actually the LED is on; this is because
                                    // it is active low on the ESP-01)
  delay(1000);                      // Wait for a second
  digitalWrite(2, HIGH);            // Turn the LED off by making the voltage HIGH
  digitalWrite(5,HIGH);
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}
