#include "Ota.h"
#include "ESP8266httpUpdate.h"
#include "ESP8266WiFi.h"
#include "Utitlity.h"

uint8_t OTA::start(const char *fileUrl)
{
    Debug("OTA :: Receiving OTA: ");
    Debug(fileUrl);
    DebugLn("...");

    WiFiClient otaWifi;
    t_httpUpdate_return t = ESPhttpUpdate.update(otaWifi, fileUrl);
    if (t == HTTP_UPDATE_FAILED) 
    {
        return -1;
    }
    if (t == HTTP_UPDATE_NO_UPDATES)
    {
        return 1;
    }
    return 0;
}