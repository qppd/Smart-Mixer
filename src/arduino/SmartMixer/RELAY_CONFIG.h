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
void operateRELAY(int RELAY, bool OPENED);

//-----------------------------------------------------------------
//FUNCTION FOR OPERATING SOLID STATE RELAY-------------------------
//-----------------------------------------------------------------
void operateSSR(int RELAY, bool OPENED);

// Unified relay operation function
void operateRelay(int RELAY, bool OPENED, bool isSSR);

#endif