#ifndef DHT_CONFIG_H
#define DHT_CONFIG_H

#include "DHT.h"
#include "PINS_CONFIG.h"

//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22  // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

extern DHT dht;

void initDHT();

/*
  Function/Method for getting DHT temperature
  
*/
float getDHTTemperature(boolean isFarenheit);

/*
  Function/Method for getting DHT humidity
*/
float getDHTHumidity();

#endif