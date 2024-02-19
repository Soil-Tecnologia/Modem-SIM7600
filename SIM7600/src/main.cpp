#include "modem_start.h"
#include "gprs_connection.h"

TinyGsm modem(Serial1);
TinyGsmClient client(modem);

void setup()
{
  start_modem();
}

void loop()
{
  init_gprs_connection();
}