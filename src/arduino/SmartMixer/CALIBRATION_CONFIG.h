#ifndef CALIBRATION_CONFIG_H
#define CALIBRATION_CONFIG_H

//-----------------------------------------------------------------
//PH SENSOR CALIBRATION VARIABLES----------------------------------
//-----------------------------------------------------------------
// pH calibration parameters
// These values need to be calibrated using known pH solutions
extern float PH_OFFSET;  // Voltage offset at pH 7
extern float PH_SLOPE;   // Slope for pH calculation (pH per volt)

//-----------------------------------------------------------------
//HX711 LOADCELL CALIBRATION VARIABLES----------------------------
//-----------------------------------------------------------------
// HX711 calibration parameters
extern float HX711_SCALE_FACTOR;  // Scale factor obtained from calibration
extern float HX711_OFFSET;        // Offset weight (tare)

//-----------------------------------------------------------------
//DHT SENSOR CALIBRATION VARIABLES---------------------------------
//-----------------------------------------------------------------
// DHT calibration parameters for sensor accuracy adjustment
extern float DHT_TEMP_OFFSET;        // Temperature offset correction (°C)
extern float DHT_HUMIDITY_OFFSET;    // Humidity offset correction (%)

#endif