#include "modem_start.h"
#include "gprs_connection.h"
#include "mqtt_file.h"
#include <Arduino.h>
#include "esp_task_wdt.h"
#include "tasks.h"

/*
    Funcao responsavel pela verificacao de queda de network e gprs, e responsavel pela conexao
    de ambos
*/
void task_gprs_connection(void *arg);

uint8_t cont = 0;

void task_gprs_connection(void *arg)
{
    Serial.println("[MODEM] Tentando se conectar");
    bool isConnected = false;
    while (true)
    {
        esp_task_wdt_reset();
        if (!modem.isGprsConnected() || !modem.isNetworkConnected())
        {
            for (int i = 0; i < 4; i++)
            {
                uint8_t network[] = {
                    38, /*Automatic*/
                    13, /*GSM only*/
                    2,  /*LTE only*/
                    51  /*GSM and LTE only*/
                };

                Serial.printf("Try %d method\n", network[i]);
                Serial.println();
                modem.setNetworkMode(network[i]);
                int tryCount = 10;
                while (tryCount--)
                {
                    esp_task_wdt_reset();
                    Serial.print("isNetworkConnected: ");
                    isConnected = modem.isNetworkConnected();
                    Serial.println(isConnected ? "CONNECT" : "NO CONNECT");
                    if (isConnected)
                    {
                        Serial.println("\n---Starting GPRS TEST---\n");
                        Serial.println("Connecting to: " + String(APN));
                        if (!modem.gprsConnect(APN, USER, PASS))
                        {
                            Serial.println("[MODE] APN Fail");
                            vTaskDelay(pdMS_TO_TICKS(3000));
                        }

                        Serial.print("GPRS status: ");
                        if (modem.isGprsConnected())
                        {
                            esp_task_wdt_reset();

                            Serial.println("connected");
                            String info = get_modem_info();
                            Serial.println(info);
                            vTaskDelay(pdMS_TO_TICKS(500));
                            if(modem.isNetworkConnected())
                            {
                                break;
                            }
                        }
                        else
                        {
                            esp_task_wdt_reset();
                            Serial.println("not connected");
                            vTaskDelay(pdMS_TO_TICKS(1000));
                        }
                    }
                    vTaskDelay(pdMS_TO_TICKS(1000));
                }
                if (modem.isGprsConnected() && modem.isNetworkConnected())
                {
                    xQueueSend(taskQueue, &task4Handle, portMAX_DELAY);
                    vTaskSuspend(NULL);
                    break;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    vTaskDelay(pdMS_TO_TICKS(100));
}
