#include <Arduino.h>
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
void operateRELAY(int RELAY, bool OPENED) {
  operateRelay(RELAY, OPENED, false);
}

//-----------------------------------------------------------------
//FUNCTION FOR OPERATING SOLID STATE RELAY-------------------------
//-----------------------------------------------------------------
void operateSSR(int RELAY, bool OPENED) {
  operateRelay(RELAY, OPENED, true);
}

//-----------------------------------------------------------------
//UNIFIED RELAY OPERATION FUNCTION---------------------------------
//-----------------------------------------------------------------
void operateRelay(int RELAY, bool OPENED, bool isSSR) {
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