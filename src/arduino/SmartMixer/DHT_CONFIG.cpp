#include <Arduino.h>
#include "DHT_CONFIG.h"

DHT dht(TEMP_SENSOR_PIN, DHTTYPE);

void initDHT() {
  dht.begin();
  Serial.println("DHT22 Temperature & Humidity Sensor: Initialized!");
}



/*
  Function/Method for getting DHT22 temperature
*/
float getDHTTemperature(boolean isFarenheit) {
  float temperature;
  if (isFarenheit) {
    temperature = dht.readTemperature(true);
  } else {
    temperature = dht.readTemperature();
  }
  if (isnan(temperature)) {
    Serial.println("Error: Failed to read temperature!");
    return -1;
  }
  return temperature;
}


/*
  Function/Method for getting DHT22 humidity
*/
float getDHTHumidity() {
  float humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Error: Failed to read humidity!");
    return -1;
  }
  return humidity;
}