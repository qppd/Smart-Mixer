#ifndef RELAY_CONFIG_H
#define RELAY_CONFIG_H

#include "PINS_CONFIG.h"

void initRELAY();

void operateRELAY(uint16_t RELAY, boolean OPENED);

void operateSSR(uint16_t RELAY, boolean OPENED);

#endif
