#pragma once
#include <Arduino.h>

#define MQTT_SERVER_URL "test.mosquitto.org"
#define MQTT_SERVER_PORT 1883

#define MQTT_CLIENT_ID "IoT-Lamp"


namespace Mqtt {
    bool isConnected();
    void tick();
}
