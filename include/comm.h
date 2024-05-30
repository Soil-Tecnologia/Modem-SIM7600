// void handle_cloud_to_board_idp(char *message);
#include <Arduino.h>


void task_comm_with_board_while_starting(void *pvParameters);
void task_serial_board(void *pvParameters);
void handle_cloud_to_board_idp(char *message);
void publish_board_to_cloud_idp();