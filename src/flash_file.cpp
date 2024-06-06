#include "SPIFFS.h"
#include "mqtt_config.h"
#include <Arduino.h>

String flash_file_read(const char *path);
void flash_file_write(const char *path, const char *message);

bool flash_file_init(void)
{
  bool init = true;
  String gprs_id = MQTT_TOPIC_DEFAULT;

  if (SPIFFS.begin(init))
  {
    Serial.println("[FLASH] OK");
    gprs_id = flash_file_read("/pivo_id.txt");
    Serial.println(gprs_id);
  }
  else
  {
    Serial.println("[FLASH] Fail");
    init = false;
  }

  return init;
}

String flash_file_read(const char *path)
{
  File file = SPIFFS.open(path, "r");
  if (!file || file.isDirectory())
  {
    return MQTT_TOPIC_DEFAULT;
  }

  const size_t bufferSize = 512;
  char buffer[bufferSize];
  String fileContent;

  while (file.available())
  {
    size_t bytesRead = file.readBytes(buffer, bufferSize);
    fileContent += String(buffer);
  }

  file.close();

  return fileContent;
}

void flash_file_write(const char *path, const char *message)
{
  File file = SPIFFS.open(path, "w");

  if (!file)
  {
    Serial.println("[FLASH] failed to save or nothing saved");
    return;
  }
  else
  {
    if (file.print(message))
    {
      Serial.println("[FLASH] Saved");
    }
    else
    {
      Serial.println("[FLASH] Not Saved");
    }
    file.close();
  }
}
