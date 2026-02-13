#include "DHT_CONFIG.h"
#include "LCD_CONFIG.h"
#include "RELAY_CONFIG.h"
#include "HX711_CONFIG.h"
#include "PH_CONFIG.h"
#include "SD_CONFIG.h"
#include "BUTTON_CONFIG.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Smart Mixer");
  initDHT();
  initLCD();

  clearLCD();
  setLCDText("Smart Mixer", 0, 0);

  initRELAY();
  initLOADCELL();
  initSD();
  initBUTTONS();
}

void loop() {
  setInputFlags();
  resolveInputFlags();

  // Serial.print("Temperature: ");
  // Serial.print(getDHTTemperature(false));
  // Serial.print(" --- ");
  // Serial.print("Weight: ");
  // Serial.print(getLOADCELLWeight());
  // Serial.print(" --- ");
  // Serial.print("pH: ");
  // Serial.print(getPHValue());
  // Serial.println(" --- ");
  // setLCDText("Temp: ", 0, 0);
  // setLCDText(getDHTTemperature(false), 6, 0);
  // Serial.print("RELAY 1: ON");
  // Serial.print(" --- ");
  // operateRELAY(RELAY_1, true);
  // Serial.print("RELAY 2: ON");
  // Serial.println(" --- ");
  // operateSSR(RELAY_2, true);

  //Write sensor data to SD card
  String data = "Temp: " + String(getDHTTemperature(false)) + ", Weight: " + String(getLOADCELLWeight()) + ", pH: " + String(getPHValue());
  writeToSD("sensor_data.txt", data);

  //delay(1000);

  // Serial.print("Temperature: ");
  // Serial.print(getDHTTemperature(false));
  // Serial.print(" --- ");
  // Serial.print("Weight: ");
  // Serial.print(getLOADCELLWeight());
  // Serial.print(" --- ");
  // Serial.print("pH: ");
  // Serial.print(getPHValue());
  // Serial.println(" --- ");
  // setLCDText("Temp: ", 0, 0);
  // setLCDText(getDHTTemperature(false), 6, 0);
  // Serial.print("RELAY 1: OFF");
  // Serial.print(" --- ");
  // operateRELAY(RELAY_1, false);
  // Serial.print("RELAY 2: OFF");
  // Serial.println(" --- ");
  // operateSSR(RELAY_2, false);
  //delay(1000);
}