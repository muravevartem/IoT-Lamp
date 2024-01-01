#include "WiFiTool.h"
#include "ESP8266WiFi.h"
#include "Timer.h"
#include "Utitlity.h"
#include "EEPROM.h"
#include "EEPROMAddress.h"

#define WIFI_AP_MODE 0x02
#define WIFI_STA_MODE 0x03

void defaultConnectingHandler()
{
    DebugLn("Connecting ...");
}

void defaultConnectedHandler()
{
    DebugLn("Connected!");
}

void defaultErrorConnectionHandler()
{
    DebugLn("Connecting failed");
}

WiFiTool::WiFiTool()
{
    connectingHandler(defaultConnectingHandler);
    connectedHandler(defaultConnectedHandler);
    errorConnectionHandler(defaultErrorConnectionHandler);
}

const char *WiFiTool::getSsid()
{
    return _ssid;
}

const char *WiFiTool::getPassword()
{
    return _pass;
}

bool WiFiTool::isSta()
{
    return _is_sta;
}

bool WiFiTool::staConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void WiFiTool::connect(const char *ssid, const char *pass)
{
    String(ssid).toCharArray(_ssid, 32);
    String(pass).toCharArray(_pass, 32);

    WiFi.softAPdisconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(_ssid, _pass);

    Debug("WiFi :: Connecting to ");
    DebugLn(_ssid);

    if (_connecting_handler != NULL)
    {
        _connecting_handler();
    }

    _is_sta = true;
    _wait_connection = true;
}

void WiFiTool::wap()
{
    WiFi.softAPdisconnect();
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
#ifdef DEFALT_WIFI_AP_SECURED
    WiFi.softAP(DEFAULT_WIFI_AP_SSID, DEFAULT_WIFI_AP_PASS);
#else
    WiFi.softAP(DEFAULT_WIFI_AP_SSID);
#endif

    Debug("WiFi :: Created WAP ");
    DebugLn(WiFi.softAPSSID());
    Debug("WiFi :: IP ");
    DebugLn(WiFi.softAPIP());

    _is_sta = false;
    _wait_connection = false;
}

void WiFiTool::initByEEPROM()
{
    EEPROM.get(WIFI_CONFIG_SSID_ADDR, _ssid);
    EEPROM.get(WIFI_CONFIG_PASS_ADDR, _pass);
    _is_sta = (EEPROM.read(WIFI_CONFIG_MODE_ADDR) == WIFI_STA_MODE);

    if (_is_sta)
    {
        connect(_ssid, _pass);
    }
    else
    {
        wap();
    }
}

void WiFiTool::commit()
{
    EEPROM.put(WIFI_CONFIG_SSID_ADDR, _ssid);
    EEPROM.put(WIFI_CONFIG_PASS_ADDR, _pass);
    EEPROM.write(WIFI_CONFIG_MODE_ADDR, _is_sta ? WIFI_STA_MODE : WIFI_AP_MODE);
}

void WiFiTool::tick()
{
    if (_is_sta)
    {
        if (_wait_connection)
        {
            if (staConnected())
            {
                DebugLn("WiFi :: Connection successful");

                if (_connected_handler != NULL)
                {
                    _connected_handler();
                }

                _interrupted = false;
                _wait_connection = false;

                commit();
            }
            if (!staConnected())
            {
                if (_connection_timer.click())
                {
                    _connection_attemps += 1;
                    if (_connection_attemps > DEFAULT_WIFI_CONNECTION_ATTEMPS)
                    {
                        DebugLn("WiFi :: Error connection");

                        if (_error_connection_handler != NULL)
                        {
                            _error_connection_handler();
                        }

                        // Не ожидаем больше соеднение и переключаемся на послднее успешное состояние
                        _wait_connection = false;
                        _connection_attemps = 0;
                        _is_sta = false;

                        initByEEPROM(); // востанавливаем последнее успешное соединение
                    }
                }
            }
        }
        else
        {

            if (!staConnected())
            {
                DebugLn("WiFi :: Unconnected");

                if (_unconnected_handler != NULL)
                {
                    _unconnected_handler();
                }

                // Помечаем соединение прерваным и ожидаем новое подключение к сети
                _interrupted = true;
                _wait_connection = true;
            }
        }
    }
}

void WiFiTool::unconnectedHandler(void (*handler)())
{
    _unconnected_handler = handler;
}

void WiFiTool::connectingHandler(void (*handler)())
{
    _connecting_handler = handler;
}

void WiFiTool::connectedHandler(void (*handler)())
{
    _connected_handler = handler;
}

void WiFiTool::errorConnectionHandler(void (*handler)())
{
    _error_connection_handler = handler;
}

WiFiTool WIFITool = WiFiTool();

WiFiClient WIFIClient = WiFiClient();