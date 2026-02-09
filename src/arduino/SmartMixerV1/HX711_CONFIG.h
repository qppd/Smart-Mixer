#ifndef HX711_CONFIG_H
#define HX711_CONFIG_H

#include "PINS_CONFIG.h"
#include "HX711.h"

extern HX711 scale;

void initLOADCELL();

float getLOADCELLWeight();

void operateHX711sleep();

void operateHX711wake();

void tareLOADCELL();

void calibrateLOADCELL(float knownWeight);

float getCalibrationFactor();

void setCalibrationFactor(float factor);

#endif