#include "modem_start.h"
#include "serial_start.h"
#include "flash_file.h"
#include <Arduino.h>

/*
    Funcao criada para inicializar o modem
*/
void start_modem();

/*
    Funcao criada para resetar o moodem
*/
void restart_modem();

void start_modem()
{
    Serial.println("[MODEM] Initializing");
    pinMode(PWR_PIN, OUTPUT);
    delay(500);
    digitalWrite(PWR_PIN, HIGH);
    modem.init();
    vTaskDelay(pdMS_TO_TICKS(2000));
}

void restart_modem()
{
    Serial.println("[MODEM] Restarting...");
    digitalWrite(PWR_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(500));
    modem.restart();
    vTaskDelay(pdMS_TO_TICKS(500));
    digitalWrite(PWR_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(10000));
}

String parsing_first_parameter_at(String input)
{
    int colonIndex = input.indexOf(':');
    if (colonIndex == -1)
    {
        return "Erro: Formato de string inválido";
    }

    // Encontrar a posição da primeira ',' após o ':'
    int commaIndex = input.indexOf(',', colonIndex);
    if (commaIndex == -1)
    {
        return "Erro: Formato de string inválido";
    }

    // Extrair o valor entre ':' e ',' e retorná-lo
    return input.substring(colonIndex + 2, commaIndex);
}

int convert_signal_strength(int value)
{
    if (value == 0)
        return -113;
    else if (value == 31)
        return -51;
    else if (value == 99 || value == 199)
        return 0; // not known or not detectable
    else if (value == 1)
        return -111;
    else if (value >= 2 && value <= 30)
    {
        if (value == 2)
            return -109;
        else if (value == 30)
            return -53;
        else
            return -109 + value * 2;
    }
    else if (value == 31)
        return -51;
    else if (value >= 100 && value <= 191)
        return -116 + (value - 100);
    else
        return 0; // Valor inválido
}

String get_modem_info()
{
    String cpsi;
    Serial1.println("AT+CPSI?");
    delay(500);
    if (Serial1.available())
    {
        cpsi = Serial1.readString();
    }

    String tech = parsing_first_parameter_at(cpsi);

    String csq;
    Serial1.println("AT+CSQ");
    delay(500);
    if (Serial1.available())
    {
        csq = Serial1.readString();
    }

    String rssi = parsing_first_parameter_at(csq);
    int rssi_negative = convert_signal_strength(rssi.toInt());

    String pivo_id = flash_file_read("/pivo_id.txt");

    String info = "#11-" + pivo_id + "-" + tech + rssi_negative + "$";

    return info;
}

String get_public_rssi()
{
    String csq;
    Serial1.println("AT+CSQ");
    delay(500);
    if (Serial1.available())
    {
        csq = Serial1.readString();
    }

    String rssi = parsing_first_parameter_at(csq);
    int rssi_negative = convert_signal_strength(rssi.toInt());

    String pivo_id = flash_file_read("/pivo_id.txt");

    String info_rssi = "#08-" + pivo_id + "-" + rssi_negative + "$";

    return info_rssi;
}