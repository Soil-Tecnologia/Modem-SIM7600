#include "flash_file.h"
#include "mqtt_config.h"
#include "modem_start.h"
#include "gprs_connection.h"
#include "flash_file.h"
#include "certs.h"
#include "certificates.h"
#include "comm.h"
#include <Arduino.h>
#include <SSLClient.h>
#include <PubSubClient.h>

void register_new_topic(String data_received_board);
void mqtt_message_callback(char *topic, byte *payload, unsigned int len);

TinyGsm modem(Serial1);
TinyGsmClient client(modem);

SSLClientParameters mTLS = SSLClientParameters::fromPEM(AWS_CERT_CRT, sizeof AWS_CERT_CRT, AWS_CERT_PRIVATE, sizeof AWS_CERT_PRIVATE);
SSLClient clientSSL(client, TAs, (size_t)TAs_NUM, 36);
PubSubClient mqtt(MODEM_MQTT_BROKER, MQTT_PORT, mqtt_message_callback, clientSSL);

uint8_t aws_connect_attempts = 0;

bool mqtt_is_connect()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
  uint32_t chip_id;
  for (int i = 0; i < 17; i = i + 8)
  {
    chip_id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  vTaskDelay(pdMS_TO_TICKS(100));
  char ID[15];
  snprintf(ID, sizeof(ID), "%lu", chip_id);
  bool status = mqtt.connect(ID);

  if (!status)
  {
    Serial.println("[MQTT] SSL fail");
    return false;
  }

  Serial.println("[MQTT] MQTT Config OK");
  Serial.println("");

  String subc = flash_file_read("/pivo_id.txt");
  mqtt.subscribe(subc.c_str());
  String modem_online = get_modem_info();

  Serial.println("\n[SENDING - CLOUD2]");
  Serial.println(modem_online.c_str());
  Serial.println();

  mqtt.publish(MQTT_TOPIC_INIT, modem_online.c_str());

  return mqtt.connected();
}

void mqtt_connect()
{
  if (!mqtt.connected())
  {
    while (!mqtt.connected())
    {
      if (mqtt_is_connect())
      {
        Serial.println("[MQTT] CONNECTED");
        return;
      }
      else
      {
        Serial.println("[MQTT] FAIL");
        aws_connect_attempts++;
        Serial.print("[MQTT] Connection attempts: ");
        Serial.println(aws_connect_attempts);
        Serial.println();

        // restart_modem();
        // init_gprs_connection();
        String info = get_modem_info();
        Serial.println(info);
        vTaskDelay(pdMS_TO_TICKS(2000));
        mqtt_connect();
        vTaskDelay(pdMS_TO_TICKS(1000));

        if (aws_connect_attempts >= MAX_SSL_CONNECT)
        {
          Serial.println("");
          Serial.println("");
          Serial.println("--------- [MQTT] - COUNTER MQTT OVERFLOW - Restarting ALL ----------");
          Serial.println("");
          Serial.println("");
          vTaskDelay(pdMS_TO_TICKS(3000));
          ESP.restart();
        }
        vTaskDelay(pdMS_TO_TICKS(100));
      }
    }
  }
  mqtt.loop();
  publish_board_to_cloud_idp();
}

void mqtt_message_callback(char *topic, byte *payload, unsigned int len)
{
  Serial.println();
  Serial.print("[CLOUD] - [");
  Serial.print(topic);
  Serial.print("]: ");
  for (unsigned int i = 0; i < len; i++)
  {
    Serial.write(payload[i]);
  }
  Serial.println();

  String message_received = "";
  for (unsigned int i = 0; i < len; i++)
  {
    message_received += (char)payload[i];
  }

  uint8_t message_len = message_received.length() + 1;
  char message[message_len];
  strcpy(message, message_received.c_str());

  // Call function
  handle_cloud_to_board_idp(message);
}

void register_new_topic(String data_received_board)
{
  String old_id = flash_file_read("/pivo_id.txt");
  mqtt.unsubscribe(old_id.c_str());

  char *hashtag_position = strchr(data_received_board.c_str(), '#');

  if (hashtag_position != NULL)
  {
    uint8_t id_position = data_received_board.indexOf("-");
    uint8_t apn_position = data_received_board.indexOf("-", id_position + 1);
    String id = data_received_board.substring(id_position + 1, apn_position);
    String apn = data_received_board.substring(apn_position + 1);

    if (id.length() < MINIMAL_SIZE_GPRS_ID)
    {
      id = flash_file_read("/pivo_id.txt").c_str();
      Serial.print("[MODEM] Invalid ID, id registered: ");
      Serial.println(id.c_str());
      String invalid_idp = "#99-Invalid_idp-" + old_id + "$";
      mqtt.publish(MQTT_TOPIC_NETWORK, invalid_idp.c_str());
    }
    else
    {
      flash_file_write("/pivo_id.txt", id.c_str());
      vTaskDelay(pdMS_TO_TICKS(100));
    }
    String new_id = flash_file_read("/pivo_id.txt");
    String subs = id.c_str();
    char subc[id.length()];
    strcpy(subc, subs.c_str());

    String new_topic = "#06-" + new_id + "new_id$";
    mqtt.publish(MQTT_TOPIC_NETWORK, new_topic.c_str(), new_topic.length());

    Serial.print("[BOARD] New Topic: ");
    Serial.println(subc);
    mqtt.subscribe(subc, qos_1);

    Serial.print("[MODEM] APN: ");
    Serial.println(apn.c_str());
  }
}
