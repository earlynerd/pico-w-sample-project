#include <Arduino.h>
#include <Adafruit_TinyUSB.h>

const int phsensorpin = 26;

void setup()
{
  analogReadResolution(12);
  Serial.begin(115200);
}

void loop()
{
  while (true)
  {
    int reading = analogRead(phsensorpin);
    Serial.print("Raw Analog Value: ");
    Serial.println(reading);
    delay(1000);
  }
}