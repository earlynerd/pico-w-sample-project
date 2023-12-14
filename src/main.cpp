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
WiFiClientSecure client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char *myWriteAPIKey = SECRET_WRITE_APIKEY;

// Fingerprint check, make sure that the certificate has not expired.
const char *fingerprint = NULL; // use SECRET_SHA1_FINGERPRINT for fingerprint check

void setup()
{
  analogReadResolution(12);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (fingerprint != NULL)
  {
    client.setFingerprint(fingerprint);
  }
  else
  {
    client.setInsecure(); // To perform a simple SSL Encryption
  }

  ThingSpeak.begin(client); // Initialize ThingSpeak
}

void loop()
{
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  while (true)
  {
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
    delay(10000);
  }
}