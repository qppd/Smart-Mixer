#ifndef DS18B20_CONFIG_H
#define DS18B20_CONFIG_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include "PINS_CONFIG.h"

extern OneWire oneWire;
extern DallasTemperature sensors;

void initDS18B20();

/*
  Function for getting DS18B20 temperature
*/
float getDS18B20Temperature(boolean isFarenheit);

#endif
