#include <Arduino.h>
#include "BUTTON_CONFIG.h"

int inputState[BUTTON_COUNT];
int lastInputState[BUTTON_COUNT] = { LOW, LOW, LOW };
bool inputFlags[BUTTON_COUNT] = { LOW, LOW, LOW };
long lastDebounceTime[BUTTON_COUNT] = { 0, 0, 0 };
const int inputPins[BUTTON_COUNT] = { BUTTON_START, BUTTON_STOP, BUTTON_CALIBRATE };

bool firstReading = true;

void initBUTTONS() {
  for (int i = 0; i < BUTTON_COUNT; i++) {
    pinMode(inputPins[i], INPUT);
    digitalWrite(inputPins[i], HIGH);
  }
  delay(1000);
  Serial.println("Push Buttons: Initialized!");
}

//-----------------------------------------------------------------
//FUNCTION FOR SETTING BUTTON STATE--------------------------------
//-----------------------------------------------------------------
void setInputFlags() {
  if (firstReading) {
    firstReading = false;
    return;
  }
  for (int i = 0; i < BUTTON_COUNT; i++) {    // loop until i is less than BUTTON_COUNT
    int reading = digitalRead(inputPins[i]);  // read pins
    if (reading != lastInputState[i]) {       // if reading is not equal to the lastInputState
      lastDebounceTime[i] = millis();         // set lastDebounceTime equal to arduino's running time
    }
    if (millis() - lastDebounceTime[i] > DEBOUNCE_DELAY) {  // if arduino's running time and lastDebounceTime difference is greater than debounceDelay
      if (reading != inputState[i]) {                       // if reading is not equal to inputState
        inputState[i] = reading;                            // then make inputState equals to reading
        if (inputState[i] == LOW) {                         // if inputState is LOW, button is pressed (active LOW with pull-up)
          inputFlags[i] = HIGH;                             // set inputFlag to HIGH to indicate button press detected
        }
      }
    }
    lastInputState[i] = reading;  // update lastInputState for next iteration
  }                               // loop end
}

//-----------------------------------------------------------------
//FUNCTION FOR READING BUTTON STATE--------------------------------
//-----------------------------------------------------------------
void resolveInputFlags() {
  for (int i = 0; i < BUTTON_COUNT; i++) {  // loop until i is less than BUTTON_COUNT
    if (inputFlags[i] == HIGH) {            //if inputFlags is HIGH then
      inputAction(i);  // do the function inputAction to navigate menu screen
      inputFlags[i] = LOW;  // set all inputFlags to LOW after pressing the switches
    }
  }  // loop end
}

// Button action handler - called when button is pressed
void inputAction(int buttonIndex) {
  // Note: Actual state transitions are handled in main loop
  // This function is called to acknowledge button press
  switch(buttonIndex) {
    case 0:  // BUTTON_START
      Serial.println("[BUTTON] START pressed!");
      break;
    case 1:  // BUTTON_STOP  
      Serial.println("[BUTTON] EMERGENCY STOP pressed!");
      break;
    case 2:  // BUTTON_CALIBRATE
      Serial.println("[BUTTON] CALIBRATE pressed!");
      break;
    default:
      Serial.print("[BUTTON] Unknown button ");
      Serial.print(buttonIndex);
      Serial.println(" pressed!");
      break;
  }
}