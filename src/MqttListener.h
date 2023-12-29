#pragma once
#include <Arduino.h>

namespace MqttListener {
    void init();
    void subscribe(const char* topic);
    void callback(char* topic, byte* payload, unsigned int length);
}