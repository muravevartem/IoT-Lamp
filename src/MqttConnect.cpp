#include "MqttConnect.h"
#include "ESP8266WiFi.h"
#include "PubSubClient.h"
#include "MqttListener.h"
#include "Utitlity.h"

extern WiFiClient _wifi_client;
PubSubClient _mqtt_client(MQTT_SERVER_URL, MQTT_SERVER_PORT, _wifi_client);

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

        if (_mqtt_client.connect(MQTT_CLIENT_ID))
        {
            Debug("Connected to ");
            Debug(MQTT_SERVER_URL);
            Debug(":");
            DebugLn(MQTT_SERVER_PORT);
            MqttListener::init();
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
