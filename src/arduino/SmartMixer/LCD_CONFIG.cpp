#include "LCD_CONFIG.h"

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setupLCD() {
    lcd.init();
    lcd.backlight();
    lcd.print("LCD Ready");
}