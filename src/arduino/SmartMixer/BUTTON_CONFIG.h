#ifndef BUTTON_CONFIG_H
#define BUTTON_CONFIG_H

#include "PINS_CONFIG.h"

extern int inputState[BUTTON_COUNT];
extern int lastInputState[BUTTON_COUNT];
extern bool inputFlags[BUTTON_COUNT];
extern long lastDebounceTime[BUTTON_COUNT];
extern const int inputPins[BUTTON_COUNT];

void initBUTTONS();
void setInputFlags();
void resolveInputFlags();
void inputAction(int buttonIndex);

#endif