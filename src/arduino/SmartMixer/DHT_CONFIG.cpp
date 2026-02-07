#include "DHT_CONFIG.h"

DHT dht(DHT_PIN, DHTTYPE);

void initDHT() {
  dht.begin();
}

/*
  Function/Method for getting DHT temperature
*/
float getDHTTemperature(boolean isFarenheit) {
  float temperature;
  if (isFarenheit) {
    temperature = dht.readTemperature(true);
  } else {
    temperature = dht.readTemperature();
  }
  if (isnan(temperature)) return -1;
  else return temperature + DHT_TEMP_OFFSET;  // Apply calibration offset
}

/*
  Function/Method for getting DHT humidity
*/
float getDHTHumidity() {
  float humidity = dht.readHumidity();
  if (isnan(humidity)) return -1;
  else return humidity + DHT_HUMIDITY_OFFSET;  // Apply calibration offset
}