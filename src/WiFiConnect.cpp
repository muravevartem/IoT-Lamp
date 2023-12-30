#include "WiFiConnect.h"
#include "WiFiConfig.h"
#include "ESP8266WiFi.h"
#include "Led.h"

#define WIFI_AP_MODE 0x1
#define WIFI_STA_MODE 0x2

#define MAX_CONNECTION_ATTEMPTS 10

#define for_i(n) for (int i = 0; i < n; i++)
#define Debug(x) Serial.print(x)
#define DebugLn(x) Serial.println(x)

extern String _wifi_ssid;
extern String _wifi_pass;
extern uint8_t _wifi_mode;

WiFiClient _wifi_client;

void _logSuccessCreated()
{
    Debug("Created AP: ");
    DebugLn(WiFi.softAPIP());
}

bool _initApMode()
{
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_AP_SSID);
    _logSuccessCreated();
    return true;
}

void _logConnecting()
{
    Debug("Connecting to ");
    DebugLn(_wifi_ssid);
}

void _logSuccessConnected()
{
    DebugLn();
    Debug("Connected to ");
    DebugLn(_wifi_ssid);
    Debug("IP: ");
    DebugLn(WiFi.localIP());
}

bool _initStaMode()
{
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(_wifi_ssid, _wifi_pass);
    _logConnecting();
    for_i(MAX_CONNECTION_ATTEMPTS)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            _logSuccessConnected();
            return true;
        }
        delay(1000);
        Debug(".");
    }
    DebugLn();
    return false;
}


bool WiFiConnect::isConnected()
{
    if (_wifi_mode == WIFI_STA_MODE)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            return _initStaMode();
        }
        return true;
    }
    else
    {
        return true;
    }
}

bool WiFiConnect::connect()
{
    if (_wifi_mode == WIFI_STA_MODE)
    {
        if (!_initStaMode())
        {
            _wifi_mode = WIFI_AP_MODE;
            connect();
            return false;
        }
        // DeviceRegistrar::regist();
        return true;
    }
    else
    {
        return _initApMode();
    }
}