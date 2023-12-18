#pragma once
#include <Arduino.h>

namespace MqttListener {
    
    void init();

    void callback(char* topic, byte* payload, unsigned int length);

}