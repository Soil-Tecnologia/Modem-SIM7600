#include <Arduino.h>

void register_new_topic(String data_received_board);
void task_mqtt_connection(void *arg);
void set_aws_certificates();