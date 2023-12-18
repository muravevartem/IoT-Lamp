#include <Arduino.h>
#include <EEPROM.h>

#include <Timer.h>
#include <clock.h>
#include <WiFiPortal.h>
#include <WiFiConfig.h>
#include <WiFiConnect.h>
#include <MqttConnect.h>
#include <MqttProducer.h>
#include <Utitlity.h>

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
  WiFiConnect::isConnected();
  WiFiPortal::tick();
  Mqtt::tick();

  static Timer t(1000);
  if (t.click()) {
    MqttProducer::send("test/123", "ahahah");
    t.reset();
  }
}