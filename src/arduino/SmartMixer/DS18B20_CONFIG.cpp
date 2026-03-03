#include <Arduino.h>
#include "DS18B20_CONFIG.h"

OneWire oneWire(TEMP_SENSOR_PIN);
DallasTemperature sensors(&oneWire);

void initDS18B20() {
  sensors.begin();
  Serial.println("DS18B20 Temperature Sensor: Initialized!");
}



/*
  Function/Method for getting DS18B20 temperature
*/
float getDS18B20Temperature(boolean isFarenheit) {
  sensors.requestTemperatures();
  float temperature;
  if (isFarenheit) {
    temperature = sensors.getTempFByIndex(0);
  } else {
    temperature = sensors.getTempCByIndex(0);
  }
  // -127 indicates a disconnected or failed sensor
  if (temperature == DEVICE_DISCONNECTED_C || temperature <= -126.0) {
    Serial.println("Error: Failed to read temperature! Sensor disconnected.");
    return -1;
  }
  return temperature;
}
