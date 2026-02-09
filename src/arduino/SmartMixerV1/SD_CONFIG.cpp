#include <Arduino.h>
#include "SD_CONFIG.h"

File myFile;
const int chipSelect = SD_CS;

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
    Serial.println("Write done to " + filename);
  } else {
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
    myFile.println("time_sec,temp_C,ph,weight_g,volume_ml");
    myFile.close();
    Serial.println("CSV file created: " + filename);
  } else {
    Serial.println("Error creating CSV file: " + filename);
  }
}

//-----------------------------------------------------------------
//FUNCTION FOR LOGGING DATA TO CSV---------------------------------
//-----------------------------------------------------------------
void logDataToCSV(String filename, unsigned long time, float temp, float ph, float weight, float volume) {
  myFile = SD.open(filename.c_str(), FILE_WRITE);
  if (myFile) {
    // Format: time,temp,ph,weight,volume
    myFile.print(time / 1000);  // Convert ms to seconds
    myFile.print(",");
    myFile.print(temp, 2);
    myFile.print(",");
    myFile.print(ph, 2);
    myFile.print(",");
    myFile.print(weight, 2);
    myFile.print(",");
    myFile.println(volume, 2);
    
    myFile.close();
    
    // Optional: Print to serial for debugging
    Serial.print("Logged: ");
    Serial.print(time / 1000);
    Serial.print("s, T:");
    Serial.print(temp);
    Serial.print("C, pH:");
    Serial.print(ph);
    Serial.print(", W:");
    Serial.print(weight);
    Serial.print("g, V:");
    Serial.print(volume);
    Serial.println("ml");
  } else {
    Serial.println("Error writing to CSV: " + filename);
  }
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