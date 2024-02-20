#include "modem_start.h"
#include <Arduino.h>

/*
    Funcao responsavel pela verificacao de queda de network e gprs, e responsavel pela conexao
    de ambos
*/
void init_gprs_connection();

void init_gprs_connection()
{
    Serial.println("Automatic");
    modem.setNetworkMode(39);
    while (!modem.isNetworkConnected())
    {
        while (!modem.gprsConnect("virtueyes.com.br", "virtu", "virtu"))
        {
            Serial.println("Falha em conectar no GPRS");
            restart_modem();
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
    Serial.println("Falha em conectar na Network");
    vTaskDelay(pdMS_TO_TICKS(3000));
}
