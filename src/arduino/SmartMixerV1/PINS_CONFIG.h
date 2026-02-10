#ifndef PINS_CONFIG_H
#define PINS_CONFIG_H

// Temperature sensor pin (DHT22)
#define TEMP_SENSOR_PIN 2  // DHT22 sensor

// HX711 load cell amplifier pins
#define HX711_DT 4
#define HX711_SCK 3

// pH sensor pin (analog)
#define PH_PIN A0

// Relay pins
#define RELAY_GRINDER 23   // Grinding motor relay (SSR for PWM/PID)
#define RELAY_PUMP 22      // Peristaltic pump relay

// Multiple buttons configuration
#define BUTTON_COUNT 3
#define DEBOUNCE_DELAY 50

#define BUTTON_START 24     // Start process
#define BUTTON_STOP 5       // Emergency stop
#define BUTTON_CALIBRATE 6  // Calibration mode

// SD card chip select pin
#define SD_CS 53  // For Arduino Mega

// System timing
#define SENSOR_READ_INTERVAL 5000  // Read sensors every 5 seconds
#define MAX_REACTION_TIME 10800000 // 3 hours max (in milliseconds)

#endif