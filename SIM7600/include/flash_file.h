#include <Arduino.h>

/**
 * @brief Initializes the SPIFFS file system and performs necessary operations.
 * @return Returns true if initialization is successful; otherwise, returns false.
 */
bool flash_file_init(void);

/**
 * @brief Reads the content of a file in SPIFFS.
 * @param path The path to the file.
 * @return Returns the content of the file as a String.
 */
String flash_file_read(const char *path);

/**
 * @brief Writes data to a file in SPIFFS.
 * @param path The path to the file.
 * @param message The data to be written to the file.
 */
void flash_file_write(const char *path, const char *message);