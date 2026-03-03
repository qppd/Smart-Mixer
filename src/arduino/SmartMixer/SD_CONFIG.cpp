#include <Arduino.h>
#include "SD_CONFIG.h"
#include "PH_CONFIG.h"

File myFile;
const int chipSelect = SD_CS;
bool sdLastWriteOk   = false;

void initSD() {
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}

void writeToSD(String filename, String data) {
  myFile = SD.open(filename.c_str(), FILE_WRITE);
  if (myFile) {
    myFile.println(data);
    myFile.close();
    sdLastWriteOk = true;
    Serial.println("Write done to " + filename);
  } else {
    sdLastWriteOk = false;
    Serial.println("error opening " + filename);
  }
}

String readFromSD(String filename) {
  String content = "";
  myFile = SD.open(filename.c_str());
  if (myFile) {
    while (myFile.available()) {
      content += (char)myFile.read();
    }
    myFile.close();
  } else {
    Serial.println("error opening " + filename);
  }
  return content;
}

//-----------------------------------------------------------------
//FUNCTION FOR CREATING CSV FILE WITH HEADER-----------------------
//-----------------------------------------------------------------
void createCSVFile(String filename) {
  // Check if file exists
  if (SD.exists(filename.c_str())) {
    Serial.println("CSV file already exists: " + filename);
    return;
  }
  
  // Create new file with header
  myFile = SD.open(filename.c_str(), FILE_WRITE);
  if (myFile) {
    myFile.println("time_sec,temp_C,ph,weight_g,dispensed_weight_g");
    myFile.close();
    Serial.println("CSV file created: " + filename);
  } else {
    Serial.println("Error creating CSV file: " + filename);
  }
}

//-----------------------------------------------------------------
//FUNCTION FOR LOGGING DATA TO CSV---------------------------------
//-----------------------------------------------------------------
void logDataToCSV(String filename, unsigned long time, float temp, float ph, float weight, float dispensedWeight) {
  myFile = SD.open(filename.c_str(), FILE_WRITE);
  if (myFile) {
    myFile.print(time / 1000);  // Convert ms to seconds
    myFile.print(",");
    myFile.print(temp, 2);
    myFile.print(",");
    myFile.print(ph, 2);
    myFile.print(",");
    myFile.print(weight, 2);
    myFile.print(",");
    myFile.println(dispensedWeight, 2);
    
    myFile.close();
    sdLastWriteOk = true;
    
    // Optional: Print to serial for debugging
    Serial.print("Logged: ");
    Serial.print(time / 1000);
    Serial.print("s, T:");
    Serial.print(temp);
    Serial.print("C, pH:");
    Serial.print(ph);
    Serial.print(", W:");
    Serial.print(weight);
    Serial.print("g, Dispensed:");
    Serial.print(dispensedWeight);
    Serial.println("g");
  } else {
    sdLastWriteOk = false;
    Serial.println("Error writing to CSV: " + filename);
  }
}

//-----------------------------------------------------------------
// Save pH calibration slope + offset to SD card (overwrites).
// File format (ph_cal.txt):
//   Line 1: slope   (e.g. "-5.7000")
//   Line 2: offset  (e.g. "21.3400")
//-----------------------------------------------------------------
void savePHCalibration(float slope, float offset) {
  // Remove existing file so we overwrite, not append
  if (SD.exists(PH_CAL_FILENAME)) {
    SD.remove(PH_CAL_FILENAME);
  }
  myFile = SD.open(PH_CAL_FILENAME, FILE_WRITE);
  if (myFile) {
    myFile.println(slope,  4);
    myFile.println(offset, 4);
    myFile.close();
    Serial.println("pH calibration saved to SD.");
    Serial.print(  "  Slope : "); Serial.println(slope,  4);
    Serial.print(  "  Offset: "); Serial.println(offset, 4);
  } else {
    Serial.println("ERROR: Could not save pH calibration to SD!");
  }
}

//-----------------------------------------------------------------
// Load pH calibration from SD card.
// Returns true and sets slope/offset if file exists and is valid.
// Returns false (leaves slope/offset unchanged) on any error.
//-----------------------------------------------------------------
bool loadPHCalibration(float &slope, float &offset) {
  if (!SD.exists(PH_CAL_FILENAME)) {
    Serial.println("pH cal file not found - using defaults.");
    return false;
  }
  myFile = SD.open(PH_CAL_FILENAME);
  if (!myFile) {
    Serial.println("ERROR: Cannot open pH cal file.");
    return false;
  }
  String slopeLine  = myFile.readStringUntil('\n');
  String offsetLine = myFile.readStringUntil('\n');
  myFile.close();

  slopeLine.trim();
  offsetLine.trim();

  if (slopeLine.length() == 0 || offsetLine.length() == 0) {
    Serial.println("pH cal file empty or corrupt.");
    return false;
  }

  slope  = slopeLine.toFloat();
  offset = offsetLine.toFloat();

  Serial.println("pH calibration loaded from SD.");
  Serial.print(  "  Slope : "); Serial.println(slope,  4);
  Serial.print(  "  Offset: "); Serial.println(offset, 4);
  return true;
}

//-----------------------------------------------------------------
//FUNCTION FOR GETTING TIMESTAMP (SIMPLE)-------------------------
//-----------------------------------------------------------------
String getTimestamp() {
  unsigned long seconds = millis() / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  
  seconds = seconds % 60;
  minutes = minutes % 60;
  
  String timestamp = "";
  if (hours < 10) timestamp += "0";
  timestamp += String(hours) + ":";
  if (minutes < 10) timestamp += "0";
  timestamp += String(minutes) + ":";
  if (seconds < 10) timestamp += "0";
  timestamp += String(seconds);
  
  return timestamp;
}