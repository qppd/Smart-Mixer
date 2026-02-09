#ifndef FLOW_SENSOR_CONFIG_H
#define FLOW_SENSOR_CONFIG_H

#include "PINS_CONFIG.h"

// Flow sensor calibration
extern volatile unsigned long pulseCount;
extern float flowCalibrationFactor;  // Pulses per mL (to be calibrated)
extern float flowRate;
extern float totalVolume;

void initFLOWSENSOR();
void pulseCounter();  // ISR for flow sensor
float getFlowRate();
float getTotalVolume();
void resetFlowSensor();
void calibrateFlowSensor(float knownVolume, unsigned long pulses);

#endif
