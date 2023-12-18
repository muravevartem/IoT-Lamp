#include "MqttListener.h"
#include "PubSubClient.h"
#include "Utitlity.h"

#define TEST_TOPIC_1 "/topicforme/123"

extern PubSubClient _mqtt_client;

void MqttListener::init()
{
    _mqtt_client.setCallback(MqttListener::callback);
    DebugLn("Used callback function");
    _mqtt_client.subscribe(TEST_TOPIC_1);
    Debug("Subscribed on topic = ");
    DebugLn(TEST_TOPIC_1);
}

void MqttListener::callback(char* topic, byte* payload, unsigned int length)
{
    DebugLn(topic);
    String _topic(topic);
    if (_topic.equals(TEST_TOPIC_1))
    {
        DebugLn("New message in test topic");
    }
}
