#include "modem_start.h"
#include "gprs_connection.h"

TinyGsm modem(Serial1);
TinyGsmClient client(modem);

void setup()
{
  Serial.begin(115200, SERIAL_8N1, 3, 1);
  // Serial2.begin(115200, SERIAL_8N1, 19, 18);
  start_modem();

  String name = modem.getModemName();
  Serial.println(name);

  String info = modem.getModemInfo();
  Serial.println(info);
}

void loop()
{
  init_gprs_connection();
}