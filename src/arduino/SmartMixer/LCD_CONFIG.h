#ifndef LCD_CONFIG_H
#define LCD_CONFIG_H

#include <LiquidCrystal_I2C.h>

// I2C LCD configuration
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

extern LiquidCrystal_I2C lcd;

void setupLCD();
void testSerialLCD();

#endif