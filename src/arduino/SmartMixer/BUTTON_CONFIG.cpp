#include <Arduino.h>
#include "BUTTON_CONFIG.h"

int inputState[BUTTON_COUNT];
int lastInputState[BUTTON_COUNT] = { LOW, LOW, LOW };
bool inputFlags[BUTTON_COUNT] = { LOW, LOW, LOW };
long lastDebounceTime[BUTTON_COUNT] = { 0, 0, 0 };
const int inputPins[BUTTON_COUNT] = { BUTTON_1, BUTTON_2, BUTTON_3 };

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
        if (inputState[i] == HIGH) {                        // if inputState is equal to high which is Pressed switch then
          inputFlags[i] = HIGH;                             // make inputFlag equal to high
        }
      }
    }
    lastInputState[i] = reading;  // set last Input state equal to reading of every switches
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

// Placeholder for input action - implement based on your menu/navigation logic
void inputAction(int buttonIndex) {
  Serial.print("Button ");
  Serial.print(buttonIndex + 1);
  Serial.println(" pressed!");
  // Add your button action logic here
}