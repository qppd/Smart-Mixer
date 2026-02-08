#ifndef LCD_CONFIG_H
#define LCD_CONFIG_H

#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void initLCD();

void clearLCD();

void setLCDText(String text, int x, int y);
void setLCDText(double value, int x, int y);
void setLCDText(float value, int x, int y);
void setLCDText(int value, int x, int y);
void setLCDText(char text, int x, int y);

#endif