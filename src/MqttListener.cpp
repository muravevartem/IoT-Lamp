#include "MqttListener.h"
#include "PubSubClient.h"
#include "Utitlity.h"
#include "MqttProducer.h"
#include "DeviceRegistrar.h"

#include "Ota.h"

// Topics
#define UPDATES_TOPIC "lamp/updates"

extern PubSubClient _mqtt_client;

void MqttListener::init()
{
    _mqtt_client.setCallback(MqttListener::callback);
    DebugLn("MQTT :: Used callback function");
    subscribe(UPDATES_TOPIC);
}

void MqttListener::subscribe(const char *topic)
{
    _mqtt_client.subscribe(topic);
    Debug("MQTT :: Subscribed on ");
    DebugLn(topic);
}

void MqttListener::callback(char *topic, byte *payload, unsigned int length)
{
    DebugLn(topic);
    String _topic(topic);
    String _payload;
    for_i(length)
    {
        _payload += String((char)payload[i]);
    }
    _payload.toLowerCase();
    _payload.trim();

    if (_topic.equals(UPDATES_TOPIC))
    {
        uint8_t statusCode = OTA::start(_payload.c_str());
    }
}
