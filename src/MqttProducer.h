#pragma once
#include <Arduino.h>

namespace MqttProducer {
    void send(const char *topic, const char *payload);
}