#include <Arduino.h>
#include "HX711_CONFIG.h"

HX711 scale;
float calibrationFactor = 22500.3f;  // Default calibration factor

void initLOADCELL() {
  scale.begin(HX711_DT, HX711_SCK);

  scale.set_scale(calibrationFactor);  
  scale.tare();  // Reset the scale to 0

  Serial.println("HX711: Tare successful, ready!");
}

float getLOADCELLWeight() {
  float weight = scale.get_units(10);  // Average of 10 readings
  if (weight < 0)
    return 0;
  else
    return weight;
}

void operateHX711sleep() {
  scale.power_down();
  Serial.println("HX711: Sleep mode");
}

void operateHX711wake() {
  scale.power_up();
  Serial.println("HX711: Wake up");
}

//-----------------------------------------------------------------
//FUNCTION FOR TARING LOAD CELL------------------------------------
//-----------------------------------------------------------------
void tareLOADCELL() {
  Serial.println("HX711: Taring...");
  scale.tare();
  Serial.println("HX711: Tare complete!");
}

//-----------------------------------------------------------------
//AUTOMATIC CALIBRATION FUNCTION-----------------------------------
//-----------------------------------------------------------------
void calibrateLOADCELL(float knownWeight) {
  Serial.println("HX711: Starting calibration...");
  Serial.println("Remove all weight, then press any button...");
  
  // Tare the scale
  scale.tare();
  delay(1000);
  
  Serial.print("Place known weight (");
  Serial.print(knownWeight);
  Serial.println(" g) on scale...");
  
  // Wait for stable reading
  delay(3000);
  
  // Get raw reading
  long reading = scale.get_units(10);
  
  // Calculate calibration factor
  if (reading != 0) {
    calibrationFactor = (float)reading / knownWeight;
    scale.set_scale(calibrationFactor);
    
    Serial.print("HX711: Calibration complete! Factor = ");
    Serial.println(calibrationFactor);
    Serial.print("Current weight: ");
    Serial.print(getLOADCELLWeight());
    Serial.println(" g");
  } else {
    Serial.println("HX711: Calibration failed - no reading!");
  }
}

//-----------------------------------------------------------------
//GET CALIBRATION FACTOR-------------------------------------------
//-----------------------------------------------------------------
float getCalibrationFactor() {
  return calibrationFactor;
}

//-----------------------------------------------------------------
//SET CALIBRATION FACTOR-------------------------------------------
//-----------------------------------------------------------------
void setCalibrationFactor(float factor) {
  calibrationFactor = factor;
  scale.set_scale(calibrationFactor);
  Serial.print("HX711: Calibration factor set to ");
  Serial.println(calibrationFactor);
}