#include <Arduino.h>
#include <EEPROM.h>

#include <Timer.h>
#include <clock.h>
#include <WiFiPortal.h>
#include <WiFiConfig.h>
#include <WiFiConnect.h>
#include <MqttConnect.h>
#include <MqttProducer.h>
#include <Led.h>
#include <Utitlity.h>
#include <GyverButton.h>

/* Sensor button */
#define SENSOR_BTN_GND 13
#define SENSOR_BTN_PIN 15

/* Led matrix */
#define LED_PIN 2
#define LED_MATRIX_SIZE 16
#define LED_MAXBRIGHTNESS 100

#define DEBUG_ESP_PORT Serial
#define DEBUG_ESP_HTTP_CLIENT

GButton mainBtn(SENSOR_BTN_PIN);

void setup()
{
  pinMode(SENSOR_BTN_GND, OUTPUT);
  digitalWrite(SENSOR_BTN_GND, LOW);
  uint32_t seed = 0;
  for (int i = 0; i < 16; i++)
  {
    seed *= 4;
    seed += analogRead(A0) & 3;
    randomSeed(seed);
  }

  // Serial.begin(9600);
  EEPROM.begin(6000);
  WiFiConfig::init();
  WiFiConnect::connect();
  WiFiPortal::start();
  Strip.setup();
}

void loop()
{
  WiFiConnect::isConnected();
  WiFiPortal::tick();
  mainBtn.tick();
  Mqtt::tick();
  Strip.tick();
}