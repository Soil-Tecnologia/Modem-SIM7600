#include "modem_start.h"
#include "gprs_connection.h"
#include "mqtt_file.h"
#include <Arduino.h>

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
        if (!isConnected)
        {
            Serial.println(isConnected);
            isConnected = modem.isNetworkConnected();
            if (isConnected)
            {
                for (int i = 0; i < 4; i++)
                {
                    uint8_t network[] = {
                        38, /*Automatic*/
                        13, /*GSM only*/
                        2,  /*LTE only*/
                        51  /*GSM and LTE only*/
                    };

                    Serial.println("[MODEM] BBBBBBBBBBBBBBBBBBB");
                    Serial.printf("Try %d method\n", network[i]);
                    Serial.println();
                    modem.setNetworkMode(network[i]);
                    bool isConnected = false;
                    int tryCount = 10;
                    while (tryCount--)
                    {
                        Serial.print("isNetworkConnected: ");
                        isConnected = modem.isNetworkConnected();
                        Serial.println(isConnected ? "CONNECT" : "NO CONNECT");
                        if (isConnected)
                        {
                            Serial.println("\n---Starting GPRS TEST---\n");
                            Serial.println("Connecting to: " + String(APN));
                            if (!modem.gprsConnect(APN, USER, PASS))
                            {
                                vTaskDelay(pdMS_TO_TICKS(10000));
                                return;
                            }

                            Serial.print("GPRS status: ");
                            if (modem.isGprsConnected())
                            {
                                Serial.println("connected");
                                String info = get_modem_info();
                                Serial.println(info);
                                vTaskDelay(pdMS_TO_TICKS(3000));
                            }
                            else
                            {
                                Serial.println("not connected");
                            }
                            break;
                        }
                        vTaskDelay(pdMS_TO_TICKS(1000));
                    }
                    if (isConnected)
                    {
                        break;
                    }
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelay(pdMS_TO_TICKS(100));
}
