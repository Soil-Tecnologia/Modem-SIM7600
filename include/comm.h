// void handle_cloud_to_board_idp(char *message);
#include <Arduino.h>


void task_new_topic_register(void *pvParameters);
void task_serial_board(void *pvParameters);
void handle_cloud_to_board_idp(char *message);
void task_communication_board();