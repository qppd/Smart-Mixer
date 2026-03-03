#ifndef PH_CONFIG_H
#define PH_CONFIG_H

#include "PINS_CONFIG.h"

extern float calibration_slope;
extern float calibration_offset;

void initPH();
float getRawVoltage();
float getPHValue();
void setPHCalibration(float slope, float offset);

#endif