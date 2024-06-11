#include <Arduino.h>
#include "mqtt_config.h"
#include "serial_start.h"
#include "mqtt_file.h"
#include "flash_file.h"

void task_new_topic_register(void *pvParameters)
{
    String idp_06;
    while (1)
    {
        if (Serial2.availableForWrite())
        {

            Serial2.print("#06$");
            idp_06 = Serial2.readString();

            String data_received_board = idp_06;
            String idp_str = data_received_board.substring(1, 3);
            int idp_int = IDP_INVALID; // Valor padrão

            int hashtag_position = idp_06.indexOf('#');

            if (hashtag_position != -1)
            {
                try
                {
                    idp_int = idp_str.toInt();
                }
                catch (const std::exception &e)
                {
                    Serial.println("[ERROR] Empty IDP");
                }
            }

            if (idp_int == IDP_6)
            {
                register_new_topic(idp_06.c_str());
                Serial.println(idp_06);
                vTaskDelete(NULL);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    vTaskDelay(pdMS_TO_TICKS(500));
}

void handle_cloud_to_board_idp(char *message)
{
    String modem_rssi;
    String modem_tecnology;
    uint8_t number = 0;

    String data_received_cloud = message;
    String numberStr = data_received_cloud.substring(1, 3);

    char *hashtag_position = strchr(message, '#');

    if (hashtag_position != NULL)
    {
        number = numberStr.toInt();

        String new_id = flash_file_read("/pivo_id.txt");
        String cloud_empty_string = "#98-Empty_idp_received_cloud-" + new_id + "$";
        String ack_received = "#20-" + new_id + "-" + "ack_received$";
        switch (number)
        {
        case IDP_0:
            Serial2.println(message);
            break;
        case IDP_1:
            Serial2.println(message);
            break;
        case IDP_2:
            Serial2.print(message);
            break;
        case IDP_3:
            Serial2.print(message);
            break;
        case IDP_4:
            Serial2.print(message);
            break;
        case IDP_5:
            Serial2.print(message);
            break;
        case IDP_7:
            Serial2.print(message);
            break;
        case IDP_8:
            modem_rssi = get_public_rssi();
            Serial.print("[MODEM] RSSI: ");
            Serial.println(modem_rssi.c_str());
            mqtt.publish(MQTT_TOPIC_NETWORK, modem_rssi.c_str(), modem_rssi.length());
            break;
        case IDP_11:
            modem_tecnology = get_modem_info();
            Serial.print("[MODEM] TECNOLOGY: ");
            Serial.println(modem_tecnology.c_str());
            mqtt.publish(MQTT_TOPIC_NETWORK, modem_tecnology.c_str(), modem_tecnology.length());
            break;
        case IDP_13:
            Serial2.println(message);
            break;
        case IDP_14:
            Serial2.println(message);
            break;
        case IDP_15:
            Serial2.println(message);
            break;
        case IDP_16:
            Serial2.println(message);
            break;
        case IDP_17:
            Serial2.println(message);
            break;
        case IDP_20:
            // Serial.println("[MODEM] ACK RECEIVED FROM - agrishow_1");
            // mqtt.publish(MQTT_TOPIC_NETWORK, ack_received.c_str(), ack_received.length());
            break;
        case IDP_22:
            Serial2.print(message);
            break;
        case IDP_23:
            Serial2.print(message);
            break;
        case IDP_24:
            Serial2.print(message);
            break;
        case IDP_90:
            Serial2.println("#90$");
            break;
        case IDP_92:
            Serial.println("[MODEM] RESETING");
            ESP.restart();
            break;
        case IDP_99:
            Serial.println(cloud_empty_string);
            mqtt.publish(MQTT_TOPIC_NETWORK, cloud_empty_string.c_str(), cloud_empty_string.length());
            break;
        default:
            String new_id = flash_file_read("/pivo_id.txt");
            String cloud_invalid_string = "#99-Invalid_idp-" + new_id + "$";
            Serial.println(cloud_invalid_string);
            mqtt.publish(MQTT_TOPIC_NETWORK, cloud_invalid_string.c_str(), cloud_invalid_string.length());
            break;
        }
    }
}

void task_communication_board()
{
    if (Serial2.available())
    {
        String received_board = Serial2.readString().c_str();

        Serial.print("[BOARD] ");
        Serial.println(received_board.c_str());

        uint8_t number;
        uint8_t startIndex = 0;

        uint8_t hashIndex = received_board.indexOf('#', startIndex);
        uint8_t dollarIndex = received_board.indexOf('$', hashIndex);

        String gprs_id = flash_file_read("/pivo_id.txt");

        if (hashIndex != -1 && dollarIndex != -1)
        {
            String payload = received_board.substring(hashIndex, dollarIndex + 1);

            char *hashtag_position = strchr(payload.c_str(), '#');

            if (hashtag_position != NULL)
            {
                String numberStr = payload.substring(1, 3);

                number = numberStr.toInt();

                switch (number)
                {
                case IDP_0:
                    mqtt.publish(MQTT_TOPIC_PAYLOAD, payload.c_str(), true);
                    break;
                case IDP_1:
                    mqtt.publish(MQTT_TOPIC_PAYLOAD, payload.c_str(), payload.length());
                    break;
                case IDP_2:
                    mqtt.publish(MQTT_TOPIC_CONFIG, payload.c_str(), payload.length());
                    break;
                case IDP_3:
                    mqtt.publish(MQTT_TOPIC_CONFIG, payload.c_str(), payload.length());
                    break;
                case IDP_4:
                    mqtt.publish(MQTT_TOPIC_CONFIG, payload.c_str(), payload.length());
                    break;
                case IDP_5:
                    mqtt.publish(MQTT_TOPIC_CONFIG, payload.c_str(), payload.length());
                    break;
                case IDP_6:
                    register_new_topic(payload);
                    break;
                case IDP_7:
                    mqtt.publish(MQTT_TOPIC_CONFIG, payload.c_str(), payload.length());
                    break;
                case IDP_11:
                    Serial2.print("#06$");
                    break;
                case IDP_12:
                    mqtt.publish(MQTT_TOPIC_PAYLOAD, payload.c_str(), payload.length());
                    break;
                case IDP_13:
                    mqtt.publish(MQTT_TOPIC_SCHEDULING, payload.c_str(), payload.length());
                    break;
                case IDP_14:
                    mqtt.publish(MQTT_TOPIC_SCHEDULING, payload.c_str(), payload.length());
                    break;
                case IDP_15:
                    mqtt.publish(MQTT_TOPIC_SCHEDULING, payload.c_str(), payload.length());
                    break;
                case IDP_16:
                    mqtt.publish(MQTT_TOPIC_SCHEDULING, payload.c_str(), payload.length());
                    break;
                case IDP_17:
                    mqtt.publish(MQTT_TOPIC_SCHEDULING, payload.c_str(), payload.length());
                    break;
                case IDP_18:
                    mqtt.publish(MQTT_TOPIC_SCHEDULING, payload.c_str(), payload.length());
                    break;
                case IDP_22:
                    mqtt.publish(MQTT_TOPIC_CONFIG, payload.c_str(), payload.length());
                    break;
                case IDP_23:
                    mqtt.publish(MQTT_TOPIC_CONFIG, payload.c_str(), payload.length());
                    break;
                case IDP_24:
                    mqtt.publish(MQTT_TOPIC_CONFIG, payload.c_str(), payload.length());
                    break;
                case IDP_30:
                    mqtt.publish(MQTT_TOPIC_PAYLOAD, payload.c_str(), true);
                    break;
                case IDP_90:
                    payload.remove(payload.length() - 1);
                    payload = payload + VERSION_FIRMWARE;
                    mqtt.publish(MQTT_TOPIC_NETWORK, payload.c_str(), payload.length());
                    break;
                case IDP_99:
                    mqtt.publish(MQTT_TOPIC_ERROR, payload.c_str(), payload.c_str());
                    break;
                default:
                    String invalid_idp = "#255-Invalid_idp-" + gprs_id + "$";
                    Serial.println(invalid_idp);
                    mqtt.publish(MQTT_TOPIC_NETWORK, invalid_idp.c_str(), invalid_idp.length());
                    break;
                }
            }
        }
    }
    vTaskDelay(pdMS_TO_TICKS(500));
}