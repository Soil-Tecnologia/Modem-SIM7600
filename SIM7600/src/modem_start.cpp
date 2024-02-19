#include "modem_start.h"
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
    modem.restart();
}

void restart_modem()
{
    Serial.println("[MODEM] Restarting...");
    modem.restart();
}