#include "modem_start.h"

TinyGsm modem(Serial1);
TinyGsmClient client(modem);

void setup()
{
  start_modem();
}

void loop()
{

}