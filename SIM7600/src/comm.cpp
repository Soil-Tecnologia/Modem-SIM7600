#include <Arduino.h>
#include "mqtt_config.h"
#include "serial_start.h"
#include "mqtt_file.h"
#include "flash_file.h"

void task_comm_with_board_while_starting(void *pvParameters)
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
    }
}

void task_serial_board(void *pvParameters)
{
    while (1)
    {
        if (Serial2.available())
        {
            String read_board = Serial2.readString();
            String data_received_board = read_board;
            String idp_str = data_received_board.substring(1, 3);
            int idp_int = IDP_INVALID; // Valor padrão

            int hashtag_position = read_board.indexOf('#');

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
                register_new_topic(read_board);
            }
            else
            {
                Serial.println("[MODEM] Board sent idp other than six");
            }
        }
        sleep(5);
    }
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
        try
        {
            number = numberStr.toInt();
        }
        catch (const std::exception &e)
        {
            Serial.println("[MODEM] Empty IDP");
        }
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
        case IDP_90:
            mqtt.publish(MQTT_TOPIC_NETWORK, VERSION_FIRMWARE);
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