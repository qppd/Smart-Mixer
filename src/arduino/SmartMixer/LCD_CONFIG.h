#ifndef LCD_CONFIG_H
#define LCD_CONFIG_H

#include <LiquidCrystal_I2C.h>

// I2C LCD configuration
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

extern LiquidCrystal_I2C lcd;

//-----------------------------------------------------------------
//FUNCTION FOR INITIALIZING I2C LCD--------------------------------
//-----------------------------------------------------------------
void initLCD();

//-----------------------------------------------------------------
//FUNCTION FOR CLEARING MESSAGE FROM I2C LCD-----------------------
//-----------------------------------------------------------------
void clearLCD();

//-----------------------------------------------------------------
//FUNCTION FOR SETTING MESSAGE I2C LCD----------------------------
//-----------------------------------------------------------------
void setLCDText(String text, int x, int y);
void setLCDText(double value, int x, int y);
void setLCDText(float value, int x, int y);
void setLCDText(int value, int x, int y);
void setLCDText(char text, int x, int y);

#endif