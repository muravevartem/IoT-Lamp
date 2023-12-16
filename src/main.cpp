#include <Arduino.h>
#include <FastLED.h>

#include <EEPROM.h>
#include <PubSubClient.h>

#include <Timer.h>
#include <clock.h>
#include <dns.h>
#include <api.h>
#include <wifi.h>
#include <ledmatrix.h>

/* Mqtt */
#define MQTT_SERVER ""
#define MQTT_PORT 1324
// #define MQTT_USER ""
// #define MQTT_PASS ""

/* Utilities */
#define for_i(n) for (int i = 0; i < n; i++)
#define for_rev_i(n) for (int i = n - 1; i >= 0; i--)
#define for_j(n) for (int j = 0; j < n; j++)
#define for_rev_j(n) for (int j = n - 1; j >= 0; j--)



WiFiClient wifiClient;
PubSubClient mqttClient(MQTT_SERVER, MQTT_PORT, wifiClient);


void setup()
{
  initMatrix();
  Serial.begin(9600);
  EEPROM.begin(6000);

  fullColor(CRGB::AliceBlue);
  tickLed();

  if (!initWifi())
  {
    resetWifi();
  }
  initRoutes();
  initDns();
  startApi();
  
  fullColor(CRGB::Black);
  tickLed();
}

void loop()
{
  tickWiFi();
  tickApi();
  tickDNS();
}