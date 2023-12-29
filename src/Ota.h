#pragma once
#include <Arduino.h>

namespace OTA {
    /*
    -1 - error
    0 - ok
    1 - no updates
    */
    uint8_t start(const char* fileUrl);
}