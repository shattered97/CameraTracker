#include <Arduino.h>

void setup() 
{
  // put your setup code here, to run once:

  delay(1000 * 5);
  

    pinMode(13, OUTPUT);
    Serial.begin(115200);
    Serial.print("Hello World");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
  Serial.println("Hello Bob");
}