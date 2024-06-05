#include "modem_start.h"
#include "gprs_connection.h"
#include "serial_start.h"
#include "flash_file.h"
#include "comm.h"
#include "mqtt_file.h"
#include "esp_task_wdt.h"

TaskHandle_t task_new_topic_register = NULL;
TaskHandle_t task3Handle = NULL;


void setup()
{
  Serial.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL_TX, SERIAL_RX);
  Serial2.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL1_TX, SERIAL1_RX);
  Serial1.begin(SERIAL2_BAUD_RATE, SERIAL_8N1, SERIAL2_TX, SERIAL2_RX);

  Serial.println("[MODEM] INICIALIZANDO AS SERIAIS");
  vTaskDelay(pdMS_TO_TICKS(500));

  esp_task_wdt_init(30, true);

  flash_file_init();

  restart_modem();
  start_modem();

  set_aws_certificates();

  xTaskCreate(task_comm_with_board_while_starting, "TaskComm", 2048, NULL, tskIDLE_PRIORITY, &task_new_topic_register);
  xTaskCreate(task_gprs_connection, "task_gprs", 65536, NULL, configMAX_PRIORITIES, &task3Handle);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(100));
}