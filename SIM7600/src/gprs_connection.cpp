#include "modem_start.h"
#include <Arduino.h>

/*
    Funcao responsavel pela verificacao de queda de network e gprs, e responsavel pela conexao
    de ambos
*/
void init_gprs_connection();

void init_gprs_connection()
{
    while (!modem.isNetworkConnected() || !modem.isGprsConnected())
    {
        modem.setNetworkMode(2);

        if (modem.isNetworkConnected())
        {
            while (!modem.gprsConnect("virtueyes.com.br"))
            {
                restart_modem();
                vTaskDelay(pdMS_TO_TICKS(3000));
            }

            Serial.println("APN connected");

            Serial1.println("AT+CPSI?");
            delay(500);
            if (Serial1.available())
            {
                String at = Serial1.readString();
                Serial1.println(at);
            }
        }
    }
}
