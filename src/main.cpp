#include "modem_start.h"
#include "gprs_connection.h"
#include "serial_start.h"
#include "flash_file.h"
#include "comm.h"
#include "mqtt_file.h"
#include "esp_task_wdt.h"

TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
TaskHandle_t task3Handle = NULL;
TaskHandle_t task4Handle = NULL;

QueueHandle_t taskQueue;

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL_TX, SERIAL_RX);
  Serial2.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL1_TX, SERIAL1_RX);
  Serial1.begin(SERIAL2_BAUD_RATE, SERIAL_8N1, SERIAL2_TX, SERIAL2_RX);

  Serial.println("[MODEM] INICIALIZANDO AS SERIAIS");
  vTaskDelay(pdMS_TO_TICKS(500));

  taskQueue = xQueueCreate(5, sizeof(TaskHandle_t));

  esp_task_wdt_init(30, true);

  flash_file_init();

  restart_modem();
  start_modem();

  set_aws_certificates();

  xTaskCreate(task_comm_with_board_while_starting, "TaskComm", 10000, NULL, tskIDLE_PRIORITY, &task1Handle);
  xTaskCreate(task_gprs_connection, "task_gprs", 10000, NULL, configMAX_PRIORITIES - 1, &task3Handle);
  vTaskDelay(pdMS_TO_TICKS(10000));
  xTaskCreate(task_mqtt_connection, "task_mqtt", 10000, NULL, configMAX_PRIORITIES, &task4Handle);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(100));
}