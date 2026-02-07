#include "CALIBRATION_CONFIG.h"

//-----------------------------------------------------------------
//PH SENSOR CALIBRATION VARIABLES----------------------------------
//-----------------------------------------------------------------
// Default calibration values - these should be adjusted based on your sensor
float PH_OFFSET = 2.5;    // Typical offset voltage at pH 7 (adjust during calibration)
float PH_SLOPE = -5.7;    // Typical slope (negative because voltage decreases with increasing pH)

//-----------------------------------------------------------------
//HX711 LOADCELL CALIBRATION VARIABLES----------------------------
//-----------------------------------------------------------------
// Default calibration values - these should be adjusted based on your load cell
float HX711_SCALE_FACTOR = 22500.3f;  // Scale factor from previous calibration
float HX711_OFFSET = 0.0f;            // Offset weight (can be set during tare)

//-----------------------------------------------------------------
//DHT SENSOR CALIBRATION VARIABLES---------------------------------
//-----------------------------------------------------------------
// Default calibration offsets - adjust based on comparison with calibrated device
float DHT_TEMP_OFFSET = 0.0;        // Temperature offset correction (°C)
float DHT_HUMIDITY_OFFSET = 0.0;    // Humidity offset correction (%)