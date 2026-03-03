/*
 * LCD_CONFIG.cpp
 * Full I2C LCD integration for the Smart Mixer project.
 *
 * Display layout (16 cols × 4 rows):
 *   Idle      : splash screen + button hints
 *   Grinding  : current / target weight + grinder state
 *   Dispensing: dispensed / target vinegar + PID output %
 *   Monitoring: temp, pH, weight, dispensed, relay states,
 *               elapsed time + reaction status
 *   Complete  : final readings + SD card result
 *   Error     : blinking alert + corrective hint
 *
 * All display functions write every row atomically to prevent
 * partial-update ghosting. Periodic refreshes are gated by
 * lcdShouldUpdate() so the main loop is never blocked.
 */

#include <Arduino.h>
#include "LCD_CONFIG.h"

// -----------------------------------------------------------
// LCD instance  (16 cols, 4 rows, I2C address 0x27)
// -----------------------------------------------------------
LiquidCrystal_I2C lcd(LCD_I2C_ADDR, LCD_COLS, LCD_ROWS);

// -----------------------------------------------------------
// Custom character bitmaps (5×8 pixels each)
// -----------------------------------------------------------
static byte degreeChar[8] = {
  0b01100,
  0b10010,
  0b10010,
  0b01100,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

static byte alertChar[8] = {
  0b00100,
  0b01110,
  0b01110,
  0b11111,
  0b11111,
  0b00100,
  0b00000,
  0b00100
};

static byte checkChar[8] = {
  0b00000,
  0b00001,
  0b00011,
  0b10110,
  0b11100,
  0b01000,
  0b00000,
  0b00000
};

static byte arrowChar[8] = {
  0b00000,
  0b00100,
  0b00110,
  0b11111,
  0b11111,
  0b00110,
  0b00100,
  0b00000
};

static byte mixerChar[8] = {
  0b01110,
  0b10101,
  0b11111,
  0b01110,
  0b01110,
  0b11111,
  0b10101,
  0b01110
};

// -----------------------------------------------------------
// Internal helpers
// -----------------------------------------------------------

/*
 * Write exactly LCD_COLS characters to the current cursor
 * position, padding with spaces so stale characters are
 * always cleared.
 */
static void lcdPrintPadded(const char* str) {
  int len = strlen(str);
  for (int i = 0; i < LCD_COLS; i++) {
    if (i < len) lcd.write((uint8_t)str[i]);
    else         lcd.write(' ');
  }
}

/* Helper: position cursor then write a padded row. */
static void lcdRow(uint8_t row, const char* str) {
  lcd.setCursor(0, (int)row);
  lcdPrintPadded(str);
}

/*
 * Format a float to the smallest reasonable string that fits
 * in fieldWidth characters.  Uses dtostrf which is always
 * available on AVR/Arduino.
 */
static void fmtFloat(char* buf, int bufLen,
                     float val, int fieldWidth, int decimals) {
  dtostrf(val, fieldWidth, decimals, buf);
  buf[bufLen - 1] = '\0';
}

// -----------------------------------------------------------
// Public: core LCD functions
// -----------------------------------------------------------
void initLCD() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(CHAR_DEGREE,  degreeChar);
  lcd.createChar(CHAR_ALERT,   alertChar);
  lcd.createChar(CHAR_CHECK,   checkChar);
  lcd.createChar(CHAR_ARROW_R, arrowChar);
  lcd.createChar(CHAR_MIXER,   mixerChar);
  lcd.clear();
}

void clearLCD() {
  lcd.clear();
}

void setLCDText(String text, int x, int y) {
  lcd.setCursor(x, y);
  lcd.print(text);
}
void setLCDText(double value, int x, int y) {
  lcd.setCursor(x, y);
  lcd.print((float)value);
}
void setLCDText(float value, int x, int y) {
  lcd.setCursor(x, y);
  lcd.print(value);
}
void setLCDText(int value, int x, int y) {
  lcd.setCursor(x, y);
  lcd.print(value);
}
void setLCDText(char ch, int x, int y) {
  lcd.setCursor(x, y);
  lcd.write((uint8_t)ch);
}

// -----------------------------------------------------------
// Non-blocking update gate
// -----------------------------------------------------------
bool lcdShouldUpdate() {
  static unsigned long lastUpdate = 0;
  unsigned long now = millis();
  if (now - lastUpdate >= LCD_UPDATE_INTERVAL) {
    lastUpdate = now;
    return true;
  }
  return false;
}

// -----------------------------------------------------------
// STATE: Idle
// Row0: "Smart Mixer    " + mixer icon
// Row1: "Ca-Acetate Synth"
// Row2: ">START  >CALIB  "
// Row3: "  System Ready  "
// -----------------------------------------------------------
void lcdDisplayIdle() {
  // Row 0: mixer icon (1 char) + 15-char title = 16 total
  lcd.setCursor(0, 0);
  lcd.write(CHAR_MIXER);
  lcd.print(" Smart Mixer   ");  // 15 chars

  lcdRow(1, "Ca-Acetate Synth");

  // Row 2: →START  →CALIB   (1+7+1+7 = 16)
  lcd.setCursor(0, 2);
  lcd.write(CHAR_ARROW_R);
  lcd.print("START  ");          // 7 chars
  lcd.write(CHAR_ARROW_R);
  lcd.print("CALIB  ");          // 7 chars

  lcdRow(3, "  System Ready  ");
}

// -----------------------------------------------------------
// STATE: Calibration
// Row0: "== CALIBRATE =="
// Row1: step text (truncated to 16)
// Row2: "Factor:XXXXXXXXX"
// Row3: "Press to confirm"
// -----------------------------------------------------------
void lcdDisplayCalibration(const char* step, float factor) {
  lcdRow(0, "== CALIBRATE == ");

  char row1[17];
  snprintf(row1, sizeof(row1), "%-16s", step);
  lcdRow(1, row1);

  char row2[17];
  char factorBuf[10];
  dtostrf(factor, 8, 1, factorBuf);
  snprintf(row2, sizeof(row2), "Factor:%s", factorBuf);
  lcdRow(2, row2);

  lcdRow(3, "Press to confirm");
}

// -----------------------------------------------------------
// pH Calibration
// Row0: title  (e.g. "== PH BUF 4.0 ==")  – 16 chars max
// Row1: "Buf: 4.0 pH:X.XX"  target buffer + live reading
// Row2: "+UP  -DN  [CONF]"  button hint
// Row3: "Offset: XX.XX   "  current offset
// -----------------------------------------------------------
void lcdDisplayPHCalibration(const char* title, float currentPH,
                              float targetPH, float offset) {
  // Row 0: title
  char row0[17];
  snprintf(row0, sizeof(row0), "%-16s", title);
  lcdRow(0, row0);

  // Row 1: "Buf: X.X pH:X.XX"  (5+3+4+4 = 16)
  char tb[5], pb[6];
  dtostrf(targetPH,  3, 1, tb);   // " 4.0" or " 7.0"
  dtostrf(currentPH, 4, 2, pb);   // "X.XX"
  char row1[17];
  snprintf(row1, sizeof(row1), "Buf:%s pH:%s", tb, pb);
  lcdRow(1, row1);

  // Row 2: button legend
  lcdRow(2, "+UP  -DN  [CONF]");

  // Row 3: current offset value
  char ob[8];
  dtostrf(offset, 6, 2, ob);       // "XXXXXX" up to " 21.34"
  char row3[17];
  snprintf(row3, sizeof(row3), "Offset:%s", ob);
  lcdRow(3, row3);
}

// -----------------------------------------------------------
// STATE: Input Target
// Row0: "== SET TARGET =="
// Row1: "Egg:  50.0 g    "
// Row2: "Vin: 600.0 g    "
// Row3: " Waiting input.."
// -----------------------------------------------------------
void lcdDisplayInputTarget(float targetGrams, float vinegarGrams) {
  lcdRow(0, "== SET TARGET ==");

  char row1[17], row2[17];
  char tb[7], vb[7];
  dtostrf(targetGrams,  5, 1, tb);
  dtostrf(vinegarGrams, 5, 1, vb);
  snprintf(row1, sizeof(row1), "Egg:%sg    ", tb);
  snprintf(row2, sizeof(row2), "Vin:%sg    ", vb);
  lcdRow(1, row1);
  lcdRow(2, row2);

  lcdRow(3, " Waiting input..");
}

// -----------------------------------------------------------
// STATE: Grinding
// Row0: "== GRINDING ==  "
// Row1: "Cur:XX.Xg/XX.Xg "  (current / target)
// Row2: "Progress: XXX%  "
// Row3: "GND:XXX [STOP]  "
// -----------------------------------------------------------
void lcdDisplayGrinding(float current, float target, bool grinderOn) {
  lcdRow(0, "== GRINDING ==  ");

  // Row 1: cur/target  (cap values to 3 chars + 1 decimal = OK for <=999.9g)
  char row1[17];
  char cb[6], tb[6];
  dtostrf(current, 4, 1, cb);
  dtostrf(target,  4, 1, tb);
  snprintf(row1, sizeof(row1), "Cur:%sg/%sg", cb, tb);
  lcdRow(1, row1);

  // Row 2: percentage
  char row2[17];
  int pct = (target > 0) ? (int)((current / target) * 100.0f) : 0;
  if (pct > 100) pct = 100;
  snprintf(row2, sizeof(row2), "Progress: %3d%%  ", pct);
  lcdRow(2, row2);

  // Row 3: grinder state
  char row3[17];
  snprintf(row3, sizeof(row3), "GND:%-3s [STOP]  ",
           grinderOn ? "ON " : "OFF");
  lcdRow(3, row3);
}

// -----------------------------------------------------------
// STATE: Dispensing
// Row0: "=  DISPENSING  ="
// Row1: "D:XX.Xg/XXX.Xg  "  (dispensed / target)
// Row2: "PID Out:  XXX%  "
// Row3: "PMP:XXX [STOP]  "
// -----------------------------------------------------------
void lcdDisplayDispensing(float dispensed, float targetVinegar,
                          bool pumpOn, int pidPct) {
  lcdRow(0, "=  DISPENSING  =");

  char row1[17];
  char db[6], tv[6];
  dtostrf(dispensed,    4, 1, db);
  dtostrf(targetVinegar, 4, 1, tv);
  snprintf(row1, sizeof(row1), "D:%sg/%sg", db, tv);
  lcdRow(1, row1);

  char row2[17];
  if (pidPct < 0)   pidPct = 0;
  if (pidPct > 100) pidPct = 100;
  snprintf(row2, sizeof(row2), "PID Out:  %3d%%  ", pidPct);
  lcdRow(2, row2);

  char row3[17];
  snprintf(row3, sizeof(row3), "PMP:%-3s [STOP]  ",
           pumpOn ? "ON " : "OFF");
  lcdRow(3, row3);
}

// -----------------------------------------------------------
// STATE: Monitoring  (main sensor dashboard)
// Row0: "T:XX.X°C pH:X.XX"  (16 chars, ° = custom char)
// Row1: "W:XXXXg  D:XXXXg"
// Row2: "GND:XXX PMP:XXX "
// Row3: "HH:MM:SS Status "
// -----------------------------------------------------------
void lcdDisplayMonitoring(float temp, float ph, float weight,
                          float dispensed, bool grinderOn,
                          bool pumpOn, unsigned long elapsedMs) {
  // Row 0: temperature (4.1f) + degree char + pH (4.2f)
  char tb[6], pb[6];
  dtostrf(temp, 4, 1, tb);
  dtostrf(ph,   4, 2, pb);

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(tb);
  lcd.write(CHAR_DEGREE);
  lcd.print("C pH:");
  lcd.print(pb);

  // Row 1: weight + dispensed  (4.0f each)
  char wb[6], db[6];
  dtostrf(weight,    4, 0, wb);
  dtostrf(dispensed, 4, 0, db);
  char row1[17];
  snprintf(row1, sizeof(row1), "W:%sg  D:%sg", wb, db);
  lcdRow(1, row1);

  // Row 2: actuator states
  char row2[17];
  snprintf(row2, sizeof(row2), "GND:%-3s PMP:%-3s ",
           grinderOn ? "ON " : "OFF",
           pumpOn    ? "ON " : "OFF");
  lcdRow(2, row2);

  // Row 3: elapsed time HH:MM:SS + reaction phase
  unsigned long totalSec = elapsedMs / 1000UL;
  unsigned int  hh  = totalSec / 3600;
  unsigned int  mm  = (totalSec % 3600) / 60;
  unsigned int  ss  = totalSec % 60;
  char row3[17];
  snprintf(row3, sizeof(row3), "%02u:%02u:%02u Active  ", hh, mm, ss);
  lcdRow(3, row3);
}

// -----------------------------------------------------------
// STATE: Complete
// Row0: "== COMPLETE! == " + check icon
// Row1: "T:XX.X°C pH:X.XX"
// Row2: "W:XXXXg  SD:XX  "
// Row3: "  Press START   "
// -----------------------------------------------------------
void lcdDisplayComplete(float weight, float temp, float ph, bool sdOk) {
  // Row 0: check icon (1 char) + 15-char label = 16 total
  lcd.setCursor(0, 0);
  lcd.write(CHAR_CHECK);
  lcd.print(" COMPLETE!     ");  // 15 chars

  // Row 1: temp + pH
  char tb[6], pb[6];
  dtostrf(temp, 4, 1, tb);
  dtostrf(ph,   4, 2, pb);
  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(tb);
  lcd.write(CHAR_DEGREE);
  lcd.print("C pH:");
  lcd.print(pb);

  // Row 2: weight + SD status
  char wb[6];
  dtostrf(weight, 4, 0, wb);
  char row2[17];
  snprintf(row2, sizeof(row2), "W:%sg  SD:%-3s  ",
           wb, sdOk ? "OK " : "ERR");
  lcdRow(2, row2);

  lcdRow(3, "  Press START   ");
}

// -----------------------------------------------------------
// STATE: Error  (blinking alert row using millis)
// Row0: "!! [ALERT] !!   "  – blinks every 600 ms
// Row1: errorMsg (truncated to 16)
// Row2: "Actuators: OFF  "
// Row3: "Check & Restart "
// -----------------------------------------------------------
void lcdDisplayError(const char* errorMsg) {
  // Blink row 0 by alternating content
  static unsigned long lastBlink = 0;
  static bool blinkState = false;
  unsigned long now = millis();
  if (now - lastBlink >= 600) {
    lastBlink = now;
    blinkState = !blinkState;
    lcd.setCursor(0, 0);
    if (blinkState) {
      // alert icon (1) + 15-char message = 16 total
      lcd.write(CHAR_ALERT);
      lcd.print(" !! ERROR !!   ");  // 15 chars
    } else {
      lcdPrintPadded("                ");  // 16 spaces clears the row
    }
  }

  // Row 1: error message
  char row1[17];
  snprintf(row1, sizeof(row1), "%-16s", errorMsg);
  lcdRow(1, row1);

  lcdRow(2, "Actuators: OFF  ");
  lcdRow(3, "Check & Restart ");
}

// -----------------------------------------------------------
// Emergency stop overlay
// Row0: "!EMERGENCY STOP!"
// Row1: "Grinder:OFF     "
// Row2: "Pump:OFF        "
// Row3: "  Press START   "
// -----------------------------------------------------------
void lcdDisplayEmergencyStop() {
  // Row 0: alert icon (1 char) + 15-char label = 16 total
  lcd.setCursor(0, 0);
  lcd.write(CHAR_ALERT);
  lcd.print("EMERGENCY STOP!");  // 15 chars

  lcdRow(1, "Grinder:OFF     ");
  lcdRow(2, "Pump:   OFF     ");
  lcdRow(3, "  Press START   ");
}

// -----------------------------------------------------------
// Brief button feedback – writes to row 3 only.
// The next regular display refresh (every LCD_UPDATE_INTERVAL)
// will naturally overwrite this, creating a ~500 ms flash.
// -----------------------------------------------------------
void lcdDisplayButtonFeedback(int buttonIndex) {
  // Row 3: arrow icon (1 char) + 15-char message = 16 total
  lcd.setCursor(0, 3);
  lcd.write(CHAR_ARROW_R);
  switch (buttonIndex) {
    case 0:  lcd.print("START pressed! "); break;  // 15 chars
    case 1:  lcd.print("STOP pressed!  "); break;  // 15 chars
    case 2:  lcd.print("CALIB pressed! "); break;  // 15 chars
    default: lcd.print("BTN pressed!   "); break;  // 15 chars
  }
}
