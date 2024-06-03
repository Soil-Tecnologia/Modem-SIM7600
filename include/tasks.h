#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern TaskHandle_t task1Handle;
extern TaskHandle_t task2Handle;
extern TaskHandle_t task3Handle;
extern TaskHandle_t task4Handle;

extern QueueHandle_t taskQueue;

void task1(void *pvParameter);
void task2(void *pvParameter);
void task3(void *pvParameter);
void task4(void *pvParameter);