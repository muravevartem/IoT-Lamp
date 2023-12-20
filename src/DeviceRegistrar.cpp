#include "DeviceRegistrar.h"
#include "DeviceToken.h"
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include "Utitlity.h"

#define DT_UNREGISTED 0x0
#define DT_REGISTED 0x1

#define DT_REGISTRAT_SECRET "qwertyuiop"

extern DeviceToken _device_token;
extern WiFiClient _wifi_client;

void DeviceRegistrar::regist()
{
    if (WiFi.status() != WL_CONNECTED)
        return;
    if (_device_token.isRegisted())
    {
        HTTPClient checkClient;

        checkClient.begin(_wifi_client, "http://192.168.1.121:8080/api/v1/leds/esp8266/check");
        checkClient.addHeader("Device-Token", _device_token.getToken());
        int status = checkClient.GET();
        if (status == 200)
        {
            checkClient.end();
            DebugLn("Token checked successful");
            return;
        }
        Debug("Unknown token status=");
        DebugLn(status);
        _device_token.reset();
        checkClient.end();
    }
    HTTPClient client;
    client.begin(_wifi_client, "http://192.168.1.121:8080/api/v1/leds/esp8266");
    client.addHeader("Device-Secret", DT_REGISTRAT_SECRET);
    int status = client.GET();
    Debug("Regist response status = ");
    DebugLn(status);
    if (status != 201)
    {
        client.end();
        DebugLn("Registration error");
        return;
    }
    delay(100);
    String tokenVal = client.getString();
    _device_token.regist(&tokenVal);
    client.end();
}
