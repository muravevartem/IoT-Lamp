#include "WiFiConfig.h"
#include "EEPROM.h"


#define for_i(n) for (int i = 0; i < n; i++)
#define Debug(x) Serial.print(x)
#define DebugLn(x) Serial.println(x)

String _wifi_ssid;
String _wifi_pass;
uint8_t _wifi_mode;


struct WiFiC
{
    char ssid[16];
    char pass[16];
    uint8_t mode;
};

void WiFiConfig::init()
{
    WiFiC config;
    EEPROM.get(WIFI_EEPROM_ADDR, config);
    _wifi_ssid = String(config.ssid);
    _wifi_pass = String(config.pass);
    _wifi_mode = config.mode;

    Debug("Readed from EEPROM ");
    Debug(_wifi_ssid);
    Debug(" ");
    Debug(_wifi_pass);
    DebugLn();
}

void WiFiConfig::commit()
{
    WiFiC config;
    _wifi_ssid.toCharArray(config.ssid, 16);
    _wifi_pass.toCharArray(config.pass, 16);
    config.mode = _wifi_mode;
    EEPROM.put(WIFI_EEPROM_ADDR, config);
    EEPROM.commit();

    Debug("Saved to EEPROM ");
    Debug(config.ssid);
    Debug(" ");
    Debug(config.pass);
    DebugLn();
}
