#include "LCD CONFIG.h"

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setupLCD() {
    lcd.init();
    lcd.backlight();
    lcd.print("LCD Ready");
}

void testSerialLCD() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim(); // Remove any trailing newline
        lcd.clear();
        lcd.setCursor(0, 0);
        if (command.length() <= 16) {
            lcd.print(command);
        } else {
            lcd.print(command.substring(0, 16));
            lcd.setCursor(0, 1);
            lcd.print(command.substring(16));
        }
    }
}