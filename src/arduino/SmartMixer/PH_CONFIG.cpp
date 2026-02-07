#include <Arduino.h>
#include "PH_CONFIG.h"

//-----------------------------------------------------------------
//FUNCTION FOR GETTING PH VALUE------------------------------------
//-----------------------------------------------------------------
float getPHValue() {
  int analogValue = analogRead(PH_PIN);
  float voltage = analogValue * (5.0 / 1023.0);  // Convert to voltage (assuming 5V reference)

  // Apply calibration: pH = (voltage - offset) * slope
  float pH = (voltage - PH_OFFSET) * PH_SLOPE;

  // Ensure pH is within reasonable range (0-14)
  if (pH < 0) pH = 0;
  if (pH > 14) pH = 14;

  return pH;
}