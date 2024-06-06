#include "modem_start.h"
#include "certs.h"
#include "certificates.h"
#include <SSLClient.h>
#include <PubSubClient.h>

#define MQTT_PORT                   8883
#define MODEM_MQTT_BROKER           "a19mijesri84u2-ats.iot.us-east-1.amazonaws.com"
#define MQTT_TOPIC_DEFAULT          "PIVO_DUDU"
#define MQTT_TOPIC_INIT             "cloud2"
#define MQTT_TOPIC_PAYLOAD          "cloudv2"
#define MQTT_TOPIC_SCHEDULING       "cloudv2-scheduling"
#define MQTT_TOPIC_NETWORK          "cloudv2-network"
#define MQTT_TOPIC_ERROR            "cloudv2-error"
#define VERSION_FIRMWARE            "-master-SIM7600-v2.0$"

extern SSLClientParameters mTLS;
extern SSLClient clientSSL;
extern PubSubClient mqtt;

#define MINIMAL_SIZE_GPRS_ID        5
#define MINIMAL_SSL_CONNECT         3
#define MAX_SSL_CONNECT             10

enum IDP
{
    IDP_0 = 0,
    IDP_1,
    IDP_2,
    IDP_3,
    IDP_4,
    IDP_5,
    IDP_6,
    IDP_7,
    IDP_8,
    IDP_9,
    IDP_10,
    IDP_11,
    IDP_12,
    IDP_13,
    IDP_14,
    IDP_15,
    IDP_16,
	IDP_17,
	IDP_18,
    IDP_20 = 20,
	IDP_22 = 22,
    IDP_23 = 23,
    IDP_24 = 24,
	IDP_30 = 30,
	IDP_90 = 90,
	IDP_91,
	IDP_92,
    IDP_99 = 99,
    IDP_INVALID = 255
};

enum QoS{
    qos_0 = 0,
    qos_1 = 1,
    qos_2 = 2
};


