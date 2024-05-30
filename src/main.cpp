#include "modem_start.h"
#include "gprs_connection.h"
#include "serial_start.h"
#include "flash_file.h"
#include "comm.h"
#include "mqtt_file.h"

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL_TX, SERIAL_RX);
  // Serial2.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL1_TX, SERIAL1_RX);
  Serial1.begin(SERIAL2_BAUD_RATE, SERIAL_8N1, SERIAL2_TX, SERIAL2_RX);
  Serial.println("[MODEM] INICIALIZANDO AS SERIAIS");
  vTaskDelay(pdMS_TO_TICKS(500));
  flash_file_init();

  xTaskCreatePinnedToCore(task_comm_with_board_while_starting, "TaskComm", 10000, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(task_serial_board, "taskBoard", 10000, NULL, 1, NULL, 1);
  start_modem();

  init_gprs_connection();
}

void loop()
{
  if (modem.isGprsConnected())
  {
    mqtt_connect();
  }
  else
  {
    init_gprs_connection();
  }
}