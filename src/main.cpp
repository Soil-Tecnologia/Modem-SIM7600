#include "modem_start.h"
#include "gprs_connection.h"
#include "serial_start.h"
#include "flash_file.h"
#include "comm.h"
#include "mqtt_file.h"
#include "esp_task_wdt.h"

TaskHandle_t new_topic_register_task = NULL;
TaskHandle_t gprs_connection_task = NULL;
TaskHandle_t communication_board_task = NULL;


void setup()
{
  Serial.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL_TX, SERIAL_RX);
  Serial2.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL1_TX, SERIAL1_RX);
  Serial1.begin(SERIAL2_BAUD_RATE, SERIAL_8N1, SERIAL2_TX, SERIAL2_RX);

  Serial.println("[MODEM] INICIALIZANDO AS SERIAIS");
  vTaskDelay(pdMS_TO_TICKS(500));

  esp_task_wdt_init(180, true);

  flash_file_init();

  restart_modem();
  start_modem();

  set_aws_certificates();

  xTaskCreate(task_new_topic_register, "TaskComm", 16384, NULL, tskIDLE_PRIORITY, &new_topic_register_task);
  xTaskCreate(task_gprs_connection, "task_gprs", 100000, NULL, configMAX_PRIORITIES, &gprs_connection_task);
  xTaskCreate(task_communication_board, "TaskBoard", 100000, NULL, configMAX_PRIORITIES - 1, &communication_board_task);

}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(100));
}