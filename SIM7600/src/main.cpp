#include "modem_start.h"
#include "gprs_connection.h"
#include "serial_start.h"
#include "flash_file.h"

TinyGsm modem(Serial1);
TinyGsmClient client(modem);

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL_TX, SERIAL_RX);
  Serial2.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL1_TX, SERIAL1_RX);
  Serial1.begin(SERIAL2_BAUD_RATE, SERIAL_8N1, SERIAL2_TX, SERIAL2_RX);

  vTaskDelay(pdMS_TO_TICKS(500));
  flash_file_init();

  start_modem();

  init_gprs_connection();
}

void loop()
{
  if(modem.isGprsConnected()){
    vTaskDelay(pdMS_TO_TICKS(5000));
    Serial.println("MODEM CONECTADO NA INTERNET");
  }
}