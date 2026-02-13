#include <Arduino.h>
#include "PH_CONFIG.h"

float calibration_value = 21.34;

void initPH() {
  pinMode(PH_PIN, INPUT);
  Serial.println("pH Sensor: Initialized!");
}

float getPHValue() {
  int buffer_arr[10];
  int temp;
  unsigned long int avgval = 0;

  for(int i=0;i<10;i++) {
    buffer_arr[i]=analogRead(PH_PIN);
    delay(30);
  }

  // Sort the Analog values received in ascending order
  for(int i=0;i<9;i++) {
    for(int j=i+1;j<10;j++) {
      if(buffer_arr[i]>buffer_arr[j]) {
        temp=buffer_arr[i];
        buffer_arr[i]=buffer_arr[j];
        buffer_arr[j]=temp;
      }
    }
  }

  for(int i=2;i<8;i++) {
    avgval+=buffer_arr[i];
  }

  float volt=(float)avgval*5.0/1024/6;
  float ph_act = -5.70 * volt + calibration_value;

  return ph_act;
}