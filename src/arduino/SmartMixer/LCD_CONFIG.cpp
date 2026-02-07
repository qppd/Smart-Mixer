#include "LCD_CONFIG.h"

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

//-----------------------------------------------------------------
//FUNCTION FOR INITIALIZING I2C LCD--------------------------------
//-----------------------------------------------------------------
void initLCD(){
  lcd.init();
  lcd.backlight();
}

//-----------------------------------------------------------------
//FUNCTION FOR CLEARING MESSAGE FROM I2C LCD-----------------------
//-----------------------------------------------------------------
void clearLCD(){
  lcd.clear();
}

//-----------------------------------------------------------------
//FUNCTION FOR SETTING MESSAGE I2C LCD----------------------------
//-----------------------------------------------------------------
void setLCDText(String text, int x, int y) {
  lcd.setCursor(x, y);
  lcd.print(text);
}
void setLCDText(double value, int x, int y) {
  lcd.setCursor(x, y);
  lcd.print(value);
}
void setLCDText(float value, int x, int y) {
  lcd.setCursor(x, y);
  lcd.print(value);
}
void setLCDText(int value, int x, int y) {
  lcd.setCursor(x, y);
  lcd.print(value);
}
void setLCDText(char text, int x, int y) {
  lcd.setCursor(x, y);
  lcd.print(text);
}