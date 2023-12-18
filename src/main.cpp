#include <Arduino.h>
#include <EEPROM.h>

#include <Timer.h>
#include <clock.h>
#include <WiFiPortal.h>
#include <WiFiConfig.h>
#include <WiFiConnect.h>
#include <MqttConnect.h>

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(6000);
  WiFiConfig::init();
  WiFiConnect::connect();
  WiFiPortal::start();
}

void loop()
{
  if (WiFiConnect::isConnected())
  {
    WiFiPortal::tick();
    Mqtt::tick();
  }
}