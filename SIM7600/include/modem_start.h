
#define TINY_GSM_MODEM_SIM7600

#include "TinyGSM.h"
#include "TinyGsmClient.h"

extern TinyGsm modem;
extern TinyGsmClient client;

#define PWR_PIN 4

void start_modem();
void restart_modem();