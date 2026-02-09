#include <Arduino.h>
#include "FLOW_SENSOR_CONFIG.h"

volatile unsigned long pulseCount = 0;
float flowCalibrationFactor = 7.5;  // Default: 7.5 pulses/mL (YF-S201 typical)
float flowRate = 0.0;
float totalVolume = 0.0;
unsigned long oldTime = 0;

//-----------------------------------------------------------------
//FUNCTION FOR INITIALIZING FLOW SENSOR----------------------------
//-----------------------------------------------------------------
void initFLOWSENSOR() {
  pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
  Serial.println("Flow Sensor: Initialized!");
}

//-----------------------------------------------------------------
//INTERRUPT SERVICE ROUTINE FOR PULSE COUNTING---------------------
//-----------------------------------------------------------------
void pulseCounter() {
  pulseCount++;
}

//-----------------------------------------------------------------
//FUNCTION FOR GETTING FLOW RATE (mL/min)--------------------------
//-----------------------------------------------------------------
float getFlowRate() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - oldTime;
  
  if (elapsedTime >= 1000) {  // Calculate every second
    // Disable interrupt while calculating
    detachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN));
    
    // Calculate flow rate in mL/min
    flowRate = (pulseCount / flowCalibrationFactor) * (60000.0 / elapsedTime);
    
    // Add to total volume (in mL)
    totalVolume += (pulseCount / flowCalibrationFactor);
    
    // Reset pulse count
    pulseCount = 0;
    oldTime = currentTime;
    
    // Re-enable interrupt
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
  }
  
  return flowRate;
}

//-----------------------------------------------------------------
//FUNCTION FOR GETTING TOTAL VOLUME (mL)---------------------------
//-----------------------------------------------------------------
float getTotalVolume() {
  return totalVolume;
}

//-----------------------------------------------------------------
//FUNCTION FOR RESETTING FLOW SENSOR-------------------------------
//-----------------------------------------------------------------
void resetFlowSensor() {
  detachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN));
  pulseCount = 0;
  totalVolume = 0.0;
  flowRate = 0.0;
  oldTime = millis();
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), pulseCounter, FALLING);
  Serial.println("Flow Sensor: Reset!");
}

//-----------------------------------------------------------------
//FUNCTION FOR CALIBRATING FLOW SENSOR-----------------------------
//-----------------------------------------------------------------
void calibrateFlowSensor(float knownVolume, unsigned long pulses) {
  if (pulses > 0 && knownVolume > 0) {
    flowCalibrationFactor = (float)pulses / knownVolume;
    Serial.print("Flow Sensor Calibrated: ");
    Serial.print(flowCalibrationFactor);
    Serial.println(" pulses/mL");
  } else {
    Serial.println("Calibration failed: Invalid values");
  }
}
