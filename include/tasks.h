#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern TaskHandle_t new_topic_register_task;
extern TaskHandle_t gprs_connection_task;
extern TaskHandle_t communication_board_task;