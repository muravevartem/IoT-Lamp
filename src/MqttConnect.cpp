#include "MqttConnect.h"
#include "ESP8266WiFi.h"
#include "PubSubClient.h"

#define for_i(n) for (int i = 0; i < n; i++)
#define Debug(x) Serial.print(x)
#define DebugLn(x) Serial.println(x)

WiFiClient _wifi_client;
PubSubClient _mqtt_client(_wifi_client);

bool Mqtt::isConnected()
{
    if (WiFi.status() != WL_CONNECTED)
        return false;

    if (!_mqtt_client.connected())
    {
        Debug("Connecting to ");
        Debug(MQTT_SERVER_URL);
        Debug(":");
        DebugLn(MQTT_SERVER_PORT);

        _mqtt_client.setServer(MQTT_SERVER_URL, MQTT_SERVER_PORT);

        if (_mqtt_client.connect(MQTT_CLIENT_ID))
        {
            Debug("Connected to ");
            Debug(MQTT_SERVER_URL);
            Debug(":");
            DebugLn(MQTT_SERVER_PORT);
        }
        else
        {
            Debug("Failed with code = ");
            DebugLn(_mqtt_client.state());
        }

        return _mqtt_client.connected();
    }
    return true;
}

void Mqtt::tick()
{
    if (isConnected())
    {
        if (WiFi.status() != WL_CONNECTED)
            return;

        _mqtt_client.loop();
    }
}
