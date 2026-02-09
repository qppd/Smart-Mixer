#ifndef RELAY_CONFIG_H
#define RELAY_CONFIG_H

#include "PINS_CONFIG.h"

void initRELAY();

void operateRELAY(uint16_t RELAY, boolean OPENED);

void operateGRINDER(boolean OPENED);

void operatePUMP(boolean OPENED);

void operatePUMP_PWM(uint8_t pwmValue);  // For PID control (0-255)

#endif
