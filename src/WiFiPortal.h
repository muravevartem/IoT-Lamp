#pragma once
#include <Arduino.h>


#define PORTAL_HOST "www.lampochka.local"

#define CT_APPLICATION_JSON "application/json"
#define CT_TEXT_HTML "text/html"

namespace WiFiPortal
{
    void tick();
    void start();
}
