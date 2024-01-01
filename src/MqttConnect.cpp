#include "MqttConnect.h"
#include "WiFiTool.h"
#include "PubSubClient.h"
#include "MqttListener.h"
#include "Timer.h"
#include "Utitlity.h"

PubSubClient _mqtt_client(MQTT_SERVER_URL, MQTT_SERVER_PORT, WIFIClient);

bool Mqtt::isConnected()
{
    if (!WIFITool.staConnected())
        return false;
    
    if (!_mqtt_client.connected())
    {
        Debug("MQTT :: Connecting to ");
        Debug(MQTT_SERVER_URL);
        Debug(":");
        DebugLn(MQTT_SERVER_PORT);

        if (_mqtt_client.connect(MQTT_CLIENT_ID))
        {
            Debug("MQTT :: Connected to ");
            Debug(MQTT_SERVER_URL);
            Debug(":");
            DebugLn(MQTT_SERVER_PORT);
            MqttListener::init();
        }
        else
        {
            Debug("MQTT :: Failed with code = ");
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
        if (!WIFITool.staConnected())
            return;
        _mqtt_client.loop();
    }
}
