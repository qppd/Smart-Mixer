#ifndef LCD_CONFIG_H
#define LCD_CONFIG_H

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

// -----------------------------------------------------------
// LCD Hardware Configuration
// -----------------------------------------------------------
#define LCD_COLS             16
#define LCD_ROWS             4
#define LCD_I2C_ADDR         0x27
#define LCD_UPDATE_INTERVAL  500   // ms between non-blocking display refreshes

// -----------------------------------------------------------
// Custom Character Indices (CGRAM slots 0-7)
// -----------------------------------------------------------
#define CHAR_DEGREE    0   // ° degree symbol
#define CHAR_ALERT     1   // ▲ warning / alert
#define CHAR_CHECK     2   // ✓ ok / checkmark
#define CHAR_ARROW_R   3   // → right arrow
#define CHAR_MIXER     4   // ◎ mixer icon

extern LiquidCrystal_I2C lcd;

// -----------------------------------------------------------
// Core LCD primitives
// -----------------------------------------------------------
void initLCD();
void clearLCD();

// Overloaded helper – write value starting at column x, row y
void setLCDText(String text,  int x, int y);
void setLCDText(double value, int x, int y);
void setLCDText(float  value, int x, int y);
void setLCDText(int    value, int x, int y);
void setLCDText(char   ch,    int x, int y);

// -----------------------------------------------------------
// Non-blocking update gate
// Returns true at most once every LCD_UPDATE_INTERVAL ms.
// Call this before any periodic refresh to prevent flicker.
// -----------------------------------------------------------
bool lcdShouldUpdate();

// -----------------------------------------------------------
// State-specific full-screen display functions
// Each function writes all four rows atomically so there are
// no partial-update artefacts.
// -----------------------------------------------------------

// Idle screen – shown at startup and between processes
void lcdDisplayIdle();

// Weight calibration helper – step describes the current prompt,
// factor is the current calibration value
void lcdDisplayCalibration(const char* step, float factor);

// pH calibration helper – shown during interactive 2-point pH calibration.
// title   : row-0 label (e.g. "== PH BUF 4.0 ==")
// currentPH : live reading from sensor
// targetPH  : known buffer value (4.0 or 7.0)
// offset    : current calibration_offset being adjusted
void lcdDisplayPHCalibration(const char* title, float currentPH,
                              float targetPH, float offset);

// Target input screen – show confirmed target and derived
// vinegar quantity while waiting for user to press START
void lcdDisplayInputTarget(float targetGrams, float vinegarGrams);

// Grinding stage – real-time weight progress
void lcdDisplayGrinding(float current, float target, bool grinderOn);

// Dispensing stage – vinegar dispensed vs. target, PID output %
void lcdDisplayDispensing(float dispensed, float targetVinegar,
                          bool pumpOn, int pidPct);

// Reaction monitoring dashboard – all sensor readings + actuator
// states + elapsed time
void lcdDisplayMonitoring(float temp, float ph, float weight,
                          float dispensed, bool grinderOn,
                          bool pumpOn, unsigned long elapsedMs);

// Process complete summary screen
void lcdDisplayComplete(float weight, float temp, float ph, bool sdOk);

// Error screen – non-blocking blinking alert with message
void lcdDisplayError(const char* errorMsg);

// Emergency-stop confirmation screen
void lcdDisplayEmergencyStop();

// Brief row-3 feedback flash on button press (overwritten by
// the next scheduled display refresh automatically)
void lcdDisplayButtonFeedback(int buttonIndex);

#endif
