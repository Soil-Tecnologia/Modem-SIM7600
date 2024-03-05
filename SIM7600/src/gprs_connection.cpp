#include "modem_start.h"
#include <Arduino.h>

/*
    Funcao responsavel pela verificacao de queda de network e gprs, e responsavel pela conexao
    de ambos
*/
void init_gprs_connection();

uint8_t cont = 0;

void init_gprs_connection()
{
    bool is_connected = modem.isNetworkConnected();

    modem.setNetworkMode(2);
    if (is_connected)
    {
        if (!modem.gprsConnect("virtueyes.com.br", "virtu", "virtu"))
        {
            Serial.println("Falha em conectar no GPRS");
            restart_modem();
            init_gprs_connection();
        }
        else
        {
            Serial.println("APN connected");
        }
    }
    else
    {
        Serial.println("Falha em conectar na Network");
        cont++;
        restart_modem();
        init_gprs_connection();

        if (cont == 5)
            esp_restart();
    }

    Serial.println("Conectado");
    vTaskDelay(pdMS_TO_TICKS(1000));
}
