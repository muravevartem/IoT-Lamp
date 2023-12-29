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

#define DEBUG_ESP_PORT Serial
#define DEBUG_ESP_HTTP_CLIENT

void setup()
{
  Serial.begin(9600);
  EEPROM.begin(6000);
  WiFiConfig::init();
  WiFiConnect::connect();
  WiFiPortal::start();
  Serial.println("By OTA");
}

void loop()
{
  WiFiConnect::isConnected();
  WiFiPortal::tick();
  Mqtt::tick();
}