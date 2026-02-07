#ifndef RELAY_CONFIG_H
#define RELAY_CONFIG_H

#include "PINS_CONFIG.h"

// Relay configuration

//-----------------------------------------------------------------
//FUNCTION FOR SETTING RELAY PIN MODE------------------------------
//-----------------------------------------------------------------
void initRELAY();

//-----------------------------------------------------------------
//FUNCTION FOR OPERATING RELAY-------------------------------------
//-----------------------------------------------------------------
void operateRELAY(uint16_t RELAY, boolean OPENED);

//-----------------------------------------------------------------
//FUNCTION FOR OPERATING SOLID STATE RELAY-------------------------
//-----------------------------------------------------------------
void operateSSR(uint16_t RELAY, boolean OPENED);

// Unified relay operation function
void operateRelay(uint16_t RELAY, boolean OPENED, boolean isSSR);

#endif