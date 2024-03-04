#include "modem_start.h"
#include "serial_start.h"
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
    Serial.println("[MODEM] Starting...");
    pinMode(PWR_PIN, OUTPUT);
    vTaskDelay(pdMS_TO_TICKS(1000));
    digitalWrite(PWR_PIN, HIGH);
    modem.init();
    modem.begin();
    vTaskDelay(pdMS_TO_TICKS(10000));
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