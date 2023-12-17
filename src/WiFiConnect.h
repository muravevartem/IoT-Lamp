#pragma once
#include <Arduino.h>

#define WIFI_AP_SSID "Lampochka"

namespace WiFiConnect {
    bool connect();
    bool isConnected();
}