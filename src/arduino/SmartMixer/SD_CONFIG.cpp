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