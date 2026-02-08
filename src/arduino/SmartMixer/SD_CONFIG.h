#ifndef SD_CONFIG_H
#define SD_CONFIG_H

#include <SPI.h>
#include <SD.h>
#include "PINS_CONFIG.h"

extern File myFile;
extern const int chipSelect;

void initSD();
void writeToSD(String filename, String data);
String readFromSD(String filename);

#endif