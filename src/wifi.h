#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <TZ.h>
#include <time.h>
#include <clock.h>

#define for_i(n) for (int i = 0; i < n; i++)
#define PRINT(x) Serial.print(x)
#define LN() Serial.println()

#define WIFI_EEPROM_ADDR 128

#define WIFI_CLIENT_MODE 0x2
#define WIFI_STATION_MODE 0x0

#define MAX_CONNECTION_ATTEMPTS 15
#define DEFAULT_WIFI_SSID "Lampochka"

struct WifiCred
{
    char ssid[16];
    char pass[16];
    uint8_t mode;
};

WifiCred wifiCredential;

bool initWifi()
{
    EEPROM.get(WIFI_EEPROM_ADDR, wifiCredential);
    if (wifiCredential.mode == WIFI_CLIENT_MODE)
    {
        WiFi.disconnect();
        WiFi.mode(WIFI_STA);
        WiFi.begin(wifiCredential.ssid, wifiCredential.pass);
        PRINT("Connecting to ");
        PRINT(String(wifiCredential.ssid));
        LN();
        for_i(MAX_CONNECTION_ATTEMPTS)
        {
            PRINT(".");
            if (WiFi.status() == WL_CONNECTED)
            {
                PRINT("IP: ");
                PRINT(WiFi.localIP());
                LN();
                return true;
            }
            delay(500);
        }
        // if cann't be connected
        return false;
    }
    else
    {
        WiFi.disconnect();
        WiFi.mode(WIFI_AP);
        WiFi.softAP(DEFAULT_WIFI_SSID);
        PRINT("Created AP with SSID: ");
        PRINT(DEFAULT_WIFI_SSID);
        LN();
        return true;
    }
}

void changeWifiCredential(WifiCred wifiCredentialForSave)
{
    EEPROM.put(WIFI_EEPROM_ADDR, wifiCredentialForSave);
    EEPROM.commit();
    initWifi();
}

void resetWifi()
{
    WifiCred dummyCred = {"", "", WIFI_STATION_MODE};
    changeWifiCredential(dummyCred);
}

bool initTime()
{
    configTime(TZ_Europe_Saratov, "pool.ntp.org", "time.nist.gov");
    PRINT("Configuring time");
    time_t now = time(nullptr);
    uint16_t attempts = 0;
    while (now < 1000000)
    {
        PRINT(".");
        now = time(nullptr);
        delay(500);
        attempts++;
        if (attempts > 10)
        {
            PRINT("Cann't be connected to time server");
            LN();
            return false;
        }
    }
    PRINT("Current time: ");
    PRINT(now);
    LN();
    return true;
}

bool tickWiFi()
{
    if (WiFi.getMode() == WIFI_AP)
    {
        return true;
    }
    if (WiFi.status() != WL_CONNECTED)
    {
        initWifi();
        if (WiFi.getMode() == WIFI_STA)
        {
            initTime();
        }
    }
    return true;
}