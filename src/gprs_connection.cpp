#include "modem_start.h"
#include "gprs_connection.h"
#include "mqtt_file.h"
#include <Arduino.h>
#include "esp_task_wdt.h"
#include "tasks.h"
#include "comm.h"

/*
    Funcao responsavel pela verificacao de queda de network e gprs, e responsavel pela conexao
    de ambos
*/
void task_gprs_connection();

uint8_t cont = 0;

#if TINY_GSM_MODEM_SIM7000
uint8_t network[] = {
    38,
    13,
    2};
#endif
#if TINY_GSM_MODEM_SIM7600
uint8_t network[] = {
    38,
    14,
    13,
    2};
#endif

uint8_t calculate_size_of_network()
{
    size_t network_size = sizeof(network) / sizeof(network[0]);
    return network_size;
}

void task_gprs_connection()
{
    if (!modem.isGprsConnected() || !modem.isNetworkConnected())
    {
        Serial.println("[MODEM] Tentando se conectar");
        bool isConnected = false;
        uint8_t failCount = 0;

        esp_task_wdt_reset();

        for (int i = 0; i < calculate_size_of_network(); i++)
        {
            int tryCount = 5;
            while (tryCount--)
            {
                esp_task_wdt_reset();
                Serial.printf("Try %d method\n", network[i]);
                modem.setNetworkMode(network[i]);
                Serial.println();
                Serial.print("is Network Connected: ");
                isConnected = modem.isNetworkConnected();
                Serial.println(isConnected ? "CONNECT" : "NO CONNECT");
                if (isConnected)
                {
#if TINY_GSM_MODEM_SIM7000
                    Serial.println("[NETWORK] Prefered modem selection: NB-iot and CAT-M");
                    modem.setPreferredMode(3);
#endif
                    Serial.println();
                    Serial.println("\n---Starting GPRS TEST---\n");
                    Serial.println("Connecting to: " + String(APN));
                    if (!modem.gprsConnect(APN, USER, PASS))
                    {
                        failCount++;
                        Serial.println("[MODE] APN Fail");
                        vTaskDelay(pdMS_TO_TICKS(3000));
                        if (failCount >= 10)
                        {
                            failCount = 0;
                            i = (i + 1) % calculate_size_of_network();
                        }
                    }

                    Serial.print("GPRS status: ");
                    if (modem.isGprsConnected())
                    {
                        esp_task_wdt_reset();

                        Serial.println("connected");
                        String info = get_modem_info();
                        Serial.println(info);
                        vTaskDelay(pdMS_TO_TICKS(500));
                        if (modem.isNetworkConnected())
                        {
                            break;
                        }
                    }
                    else
                    {
                        esp_task_wdt_reset();
                        Serial.println("not connected");
                        Serial.println();
                        vTaskDelay(pdMS_TO_TICKS(1000));
                    }
                }
                else
                {
                    cont++;
                    if (cont >= 100)
                    {
                        ESP.restart();
                    }
                }
                vTaskDelay(pdMS_TO_TICKS(2000));
            }
            if (modem.isGprsConnected() && modem.isNetworkConnected())
            {
                break;
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
