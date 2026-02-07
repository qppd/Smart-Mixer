#ifndef DHT_CONFIG_H
#define DHT_CONFIG_H

#include "PINS_CONFIG.h"
#include "DHT.h"

// DHT sensor configuration
#define DHTTYPE DHT22  // DHT 22 (AM2302), AM2321

extern DHT dht;

//-----------------------------------------------------------------
//FUNCTION FOR INITIALIZING DHT SENSOR-----------------------------
//-----------------------------------------------------------------
void initDHT();

//-----------------------------------------------------------------
//FUNCTION FOR GETTING DHT TEMPERATURE----------------------------
//-----------------------------------------------------------------
float getDHTTemperature(boolean isFarenheit);

//-----------------------------------------------------------------
//FUNCTION FOR GETTING DHT HUMIDITY-------------------------------
//-----------------------------------------------------------------
float getDHTHumidity();

#endif