#include "SPIFFS.h"
#include "mqtt_config.h"
#include <Arduino.h>

bool flash_file_init(void)
{
    bool init = true;
    String gprs_id;

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
        fileContent += String(buffer, bytesRead);
    }

    file.close();

    return fileContent;
}

void flash_file_write(const char *path, const char *message) 
{
  File file = SPIFFS.open(path, "w");
  int wifi_written = 0;

  if (!file) 
  {
    Serial.println("[FLASH] failed to open file for writing");
  } 
  else 
  {
    if (file.print(message)) 
    {
      Serial.println("[FLASH] File written");
      wifi_written++;

      if (wifi_written == 2) 
      {
        Serial.println("RESTARTING ESP TO SAVE THE NEW WIFI");
        file.close();
        delay(3000);
        esp_restart();
      }
    } 
    else 
    {
      Serial.println("[FLASH] write failed");
    }
    
    file.close();
  }
}