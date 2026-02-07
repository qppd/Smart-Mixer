#include "LCD_CONFIG.h"
#include "DHT_CONFIG.h"
#include "PH_CONFIG.h"
#include "HX711_CONFIG.h"
#include "RELAY_CONFIG.h"

void setup() {
    Serial.begin(9600);
    initLCD();
    initDHT();
    initLOADCELL();
    initRELAY();

    setLCDText("System Ready", 0, 0);
    Serial.println("Smart Mixer System Ready");
    Serial.println("Available commands:");
    Serial.println("get-temp, get-hum, relay1_on, relay1_off");
    Serial.println("relay2_on, relay2_off, get-ph, get-weight");
    Serial.println("set-lcd:<text>, stop");
}

void loop() {
    // Standby mode - just show ready message
    static bool standbyShown = false;
    if (!standbyShown) {
        setLCDText("Standby Mode", 0, 0);
        clearLCD();
        setLCDText("Standby Mode", 0, 0);
        standbyShown = true;
    }

    // Process serial commands
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();

        if (command == "stop") {
            Serial.println("System stopped");
            setLCDText("System Stopped", 0, 0);
            while (true); // Stop execution
        }

        // Process commands and show results
        if (command == "get-temp") {
            float temp = getDHTTemperature(false);
            Serial.print("Temperature: ");
            Serial.print(temp);
            Serial.println(" °C");
            setLCDText("Temp: " + String(temp) + "C", 0, 0);
        }
        else if (command == "get-hum") {
            float hum = getDHTHumidity();
            Serial.print("Humidity: ");
            Serial.print(hum);
            Serial.println(" %");
            setLCDText("Hum: " + String(hum) + "%", 0, 1);
        }
        else if (command == "relay1_on") {
            operateRELAY(RELAY_1, true);
            Serial.println("Relay 1 ON");
            setLCDText("Relay1: ON", 0, 0);
        }
        else if (command == "relay1_off") {
            operateRELAY(RELAY_1, false);
            Serial.println("Relay 1 OFF");
            setLCDText("Relay1: OFF", 0, 0);
        }
        else if (command == "relay2_on") {
            operateSSR(RELAY_2, true);
            Serial.println("Relay 2 ON");
            setLCDText("Relay2: ON", 0, 1);
        }
        else if (command == "relay2_off") {
            operateSSR(RELAY_2, false);
            Serial.println("Relay 2 OFF");
            setLCDText("Relay2: OFF", 0, 1);
        }
        else if (command == "get-ph") {
            float ph = getPHValue();
            Serial.print("pH: ");
            Serial.println(ph);
            setLCDText("pH: " + String(ph), 0, 0);
        }
        else if (command == "get-weight") {
            float weight = getLOADCELLWeight();
            Serial.print("Weight: ");
            Serial.print(weight);
            Serial.println(" units");
            setLCDText("Weight: " + String(weight), 0, 1);
        }
        else if (command.startsWith("set-lcd:")) {
            String text = command.substring(8); // Remove "set-lcd:"
            clearLCD();
            if (text.length() <= 16) {
                setLCDText(text, 0, 0);
            } else {
                setLCDText(text.substring(0, 16), 0, 0);
                setLCDText(text.substring(16), 0, 1);
            }
            Serial.println("LCD updated: " + text);
        }
        else {
            Serial.println("Unknown command: " + command);
        }
    }
}