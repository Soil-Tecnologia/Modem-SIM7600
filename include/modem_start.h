
#define TINY_GSM_MODEM_SIM7600 false
#define TINY_GSM_MODEM_SIM7000 true

#include "TinyGSM.h"
#include "TinyGsmClient.h"
#include <Arduino.h>

extern TinyGsm modem;
extern TinyGsmClient client;

#define PWR_PIN 4

void start_modem();
void restart_modem();
String get_modem_info();
String get_public_rssi();