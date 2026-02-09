#include <Arduino.h>
#include "RELAY_CONFIG.h"

//-----------------------------------------------------------------
//FUNCTION FOR SETTING RELAY PIN MODE------------------------------
//-----------------------------------------------------------------
void initRELAY(){
  pinMode(RELAY_GRINDER, OUTPUT);
  pinMode(RELAY_PUMP, OUTPUT);
  
  // Initialize to OFF state
  digitalWrite(RELAY_GRINDER, LOW);
  digitalWrite(RELAY_PUMP, LOW);
  
  Serial.println("Relays: Initialized!");
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
//FUNCTION FOR OPERATING GRINDER MOTOR-----------------------------
//-----------------------------------------------------------------
void operateGRINDER(boolean OPENED) {
  if (OPENED) {
    digitalWrite(RELAY_GRINDER, HIGH);
    Serial.println("Grinder: ON");
  } else {
    digitalWrite(RELAY_GRINDER, LOW);
    Serial.println("Grinder: OFF");
  }
}



//-----------------------------------------------------------------
//FUNCTION FOR OPERATING PUMP (ON/OFF)-----------------------------
//-----------------------------------------------------------------
void operatePUMP(boolean OPENED) {
  if (OPENED) {
    digitalWrite(RELAY_PUMP, HIGH);
    Serial.println("Pump: ON");
  } else {
    digitalWrite(RELAY_PUMP, LOW);
    Serial.println("Pump: OFF");
  }
}



//-----------------------------------------------------------------
//FUNCTION FOR OPERATING PUMP WITH PWM (PID CONTROL)---------------
//-----------------------------------------------------------------
void operatePUMP_PWM(uint8_t pwmValue) {
  // For SSR, use PWM for flow control
  // Note: Make sure RELAY_PUMP pin supports PWM
  analogWrite(RELAY_PUMP, pwmValue);
  
  // Optional: Print debug info (comment out in production for performance)
  // Serial.print("Pump PWM: ");
  // Serial.println(pwmValue);
}
