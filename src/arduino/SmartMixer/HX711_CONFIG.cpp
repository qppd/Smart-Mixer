#include "HX711_CONFIG.h"

HX711 scale;

void initLOADCELL() {
  scale.begin(HX711_DT, HX711_SCK);

  scale.set_scale(HX711_SCALE_FACTOR);  // Use calibration variable
  scale.tare();              // reset the scale to 0

  Serial.println("HX711 Tare successful");
}

float getLOADCELLWeight() {
  float weight = scale.get_units(10);
  weight -= HX711_OFFSET;  // Apply offset
  if (weight < 0)
    return 0;
  else
    return weight;
}

void operateHX711sleep() {
  scale.power_down();
}

void operateHX711wake() {
  scale.power_up();
}