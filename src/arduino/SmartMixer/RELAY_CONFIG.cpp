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
void operateRELAY(uint16_t RELAY, boolean OPENED) {
  if (OPENED)
    digitalWrite(RELAY, HIGH);
  else
    digitalWrite(RELAY, LOW);
}



//-----------------------------------------------------------------
//FUNCTION FOR OPERATING SOLID STATE RELAY-------------------------
//-----------------------------------------------------------------
void operateSSR(uint16_t RELAY, boolean OPENED) {
  if (OPENED)
    digitalWrite(RELAY, HIGH);
  else
    digitalWrite(RELAY, LOW);
}
