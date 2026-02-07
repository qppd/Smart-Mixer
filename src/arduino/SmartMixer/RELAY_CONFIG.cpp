#include "RELAY_CONFIG.h"

//-----------------------------------------------------------------
//FUNCTION FOR SETTING RELAY PIN MODE------------------------------
//-----------------------------------------------------------------
void initRELAY(){
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
}

//-----------------------------------------------------------------
//FUNCTION FOR OPERATING RELAY-------------------------------------
//-----------------------------------------------------------------
void operateRELAY(uint16_t RELAY, boolean OPENED) {
  operateRelay(RELAY, OPENED, false);
}

//-----------------------------------------------------------------
//FUNCTION FOR OPERATING SOLID STATE RELAY-------------------------
//-----------------------------------------------------------------
void operateSSR(uint16_t RELAY, boolean OPENED) {
  operateRelay(RELAY, OPENED, true);
}

//-----------------------------------------------------------------
//UNIFIED RELAY OPERATION FUNCTION---------------------------------
//-----------------------------------------------------------------
void operateRelay(uint16_t RELAY, boolean OPENED, boolean isSSR) {
  if (isSSR) {
    if (OPENED)
      digitalWrite(RELAY, HIGH);
    else
      digitalWrite(RELAY, LOW);
  } else {
    if (OPENED)
      digitalWrite(RELAY, LOW);
    else
      digitalWrite(RELAY, HIGH);
  }
}