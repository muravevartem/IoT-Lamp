#include "MqttProducer.h"
#include "PubSubClient.h"


extern PubSubClient _mqtt_client;

void MqttProducer::send(const char *topic, const char *payload)
{
    _mqtt_client.publish(topic, payload);
}