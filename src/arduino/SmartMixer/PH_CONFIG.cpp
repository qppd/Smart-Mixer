#include <Arduino.h>
#include "PH_CONFIG.h"

// Two-point calibration parameters.
// slope and offset are computed/updated by the calibration routine.
float calibration_slope  = -5.70f;
float calibration_offset = 21.34f;

void initPH() {
  pinMode(PH_PIN, INPUT);
  Serial.println("pH Sensor: Initialized!");
}

// ---------------------------------------------------------------
// Returns the averaged, sorted mid-6 voltage from the pH pin.
// Shared by both getPHValue() and the calibration routine.
// ---------------------------------------------------------------
float getRawVoltage() {
  int buffer_arr[10];
  int temp;
  unsigned long int avgval = 0;

  for (int i = 0; i < 10; i++) {
    buffer_arr[i] = analogRead(PH_PIN);
    delay(30);
  }

  // Sort ascending (bubble sort)
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buffer_arr[i] > buffer_arr[j]) {
        temp           = buffer_arr[i];
        buffer_arr[i]  = buffer_arr[j];
        buffer_arr[j]  = temp;
      }
    }
  }

  // Average the middle 6 samples (drop 2 lowest + 2 highest)
  for (int i = 2; i < 8; i++) {
    avgval += buffer_arr[i];
  }

  return (float)avgval * 5.0f / 1024.0f / 6.0f;
}

// ---------------------------------------------------------------
// Apply a new slope + offset computed from 2-point calibration.
// ---------------------------------------------------------------
void setPHCalibration(float slope, float offset) {
  calibration_slope  = slope;
  calibration_offset = offset;
}

float getPHValue() {
  float volt = getRawVoltage();
  return calibration_slope * volt + calibration_offset;
}