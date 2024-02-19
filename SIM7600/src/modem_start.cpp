#include "modem_start.h"
#include <Arduino.h>

/*
    Funcao criada para inicializar o modem
*/
void start_modem();

void start_modem()
{
    Serial.println("[MODEM] Starting...");
    modem.init();
    modem.begin();
}