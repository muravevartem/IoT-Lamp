#pragma once
#include <Arduino.h>

#define WIFI_EEPROM_ADDR 0x3

#define WIFI_AP_MODE 0x1
#define WIFI_STA_MODE 0x2

namespace WiFiConfig {
    void init();
    void commit();
}