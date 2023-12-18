#include <Arduino.h>
#include <Adafruit_TinyUSB.h>
#define TS_ENABLE_SSL // For HTTPS SSL connection

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "secrets.h"
#include "ThingSpeak.h"

const int phsensorpin = 26;

char ssid[] = SECRET_SSID; // your network SSID (name)
char pass[] = SECRET_PASS; // your network password
int keyIndex = 0;          // your network key Index number (needed only for WEP)
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char *myWriteAPIKey = SECRET_WRITE_APIKEY;


void setup()
{
  analogReadResolution(12);
  Serial.begin(115200);
  while(!Serial);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  WiFi.begin(ssid, pass);
  uint32_t wifiStartTime = millis();
  Serial.println("wifi begin");
  while((WiFi.status() != WL_CONNECTED) && (millis() - wifiStartTime < 10000));
  if(WiFi.status() == WL_CONNECTED) Serial.println("wifi connected");
  ThingSpeak.begin(client); // Initialize ThingSpeak
}

void loop()
{
  // Connect or reconnect to WiFi
  
  

    int reading = analogRead(phsensorpin);
    Serial.print("Raw Analog Value: ");
    Serial.println(reading);
    
    
    ThingSpeak.setField(1, reading);
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200)
    {
      Serial.println("Channel update successful.");
    }
    else
    {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    
    
   delay(5000);
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to reconnect to SSID: ");
    Serial.println(SECRET_SSID);
    WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
    uint32_t startTime = millis();
    while ((WiFi.status() != WL_CONNECTED) && (millis() - startTime < 8000))
    {
      Serial.print(".");
      delay(250);
    }
    if(WiFi.status() == WL_CONNECTED) Serial.println("\nConnected.");
    else Serial.println("Reconenct fail");
  }
}