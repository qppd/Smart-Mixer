#ifndef PINS_CONFIG_H
#define PINS_CONFIG_H

// DHT sensor pin
#define DHT_PIN 2

// HX711 load cell amplifier pins
#define HX711_DT 4
#define HX711_SCK 3

// pH sensor pin
#define PH_PIN A0

// Relay pins
#define RELAY_1 22  // Regular relay (Grinding motor)
#define RELAY_2 23  // SSR relay (Vinegar pump)

// Multiple buttons configuration
#define BUTTON_COUNT 3
#define DEBOUNCE_DELAY 50

#define BUTTON_1 24
#define BUTTON_2 5
#define BUTTON_3 6

// SD card chip select pin
#define SD_CS 53  // For Arduino Mega
#endif