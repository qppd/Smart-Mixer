#ifndef SD_CONFIG_H
#define SD_CONFIG_H

#include <SPI.h>
#include <SD.h>
#include "PINS_CONFIG.h"

extern File myFile;
extern const int chipSelect;

// Set to true after a successful SD write, false on failure.
// Used by LCD to display logging status (SD:OK / SD:ERR).
extern bool sdLastWriteOk;

void initSD();
void writeToSD(String filename, String data);
String readFromSD(String filename);

// CSV data logging functions
void createCSVFile(String filename);
void logDataToCSV(String filename, unsigned long time, float temp, float ph, float weight, float dispensedWeight);
String getTimestamp();

// pH calibration persistence
#define PH_CAL_FILENAME "ph_cal.txt"
void savePHCalibration(float slope, float offset);
bool loadPHCalibration(float &slope, float &offset);

#endif