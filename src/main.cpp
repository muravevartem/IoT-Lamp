#include <Arduino.h>
#include <EEPROM.h>

#include <Timer.h>
#include <WiFiPortal.h>
#include <MqttConnect.h>
#include <MqttProducer.h>
#include <Led.h>
#include <Utitlity.h>
#include <GyverButton.h>
#include <WiFiTool.h>

/* Sensor button */
#define SENSOR_BTN_GND 13
#define SENSOR_BTN_PIN 15

/* Led matrix */
#define LED_PIN 2
#define LED_MATRIX_SIZE 16
#define LED_MAXBRIGHTNESS 100

/* WiFi */
#define DEFAULT_WIFI_AP_SSID "Esp8266"
#define DEFAULT_WIFI_CONNECTION_ATTEMPS 5


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

  Serial.begin(9600);
  EEPROM.begin(6000);

  WIFITool.initByEEPROM();
  WiFiPortal::start();
  Strip.setup();
}

void loop()
{
  WIFITool.tick();
  WiFiPortal::tick();
  mainBtn.tick();
  Mqtt::tick();
  Strip.tick();
}