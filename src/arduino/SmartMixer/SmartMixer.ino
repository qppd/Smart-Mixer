#include "LCD CONFIG.h"

void setup() {
    Serial.begin(9600);
    setupLCD();
}

void loop() {
    testSerialLCD();
}