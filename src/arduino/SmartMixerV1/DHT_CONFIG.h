#ifndef DHT_CONFIG_H
#define DHT_CONFIG_H

#include "DHT.h"
#include "PINS_CONFIG.h"

#define DHTTYPE DHT22  // DHT 22 (AM2302), AM2321

extern DHT dht;

void initDHT();

/*
  Function for getting DHT22 temperature
*/
float getDHTTemperature(boolean isFarenheit);

/*
  Function for getting DHT22 humidity
*/
float getDHTHumidity();

#endif