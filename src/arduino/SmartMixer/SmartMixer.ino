#include "LCD_CONFIG.h"
#include "DHT_CONFIG.h"
#include "PH_CONFIG.h"
#include "HX711_CONFIG.h"
#include "RELAY_CONFIG.h"

// Continuous monitoring flags for each sensor
bool continuousTemp = false;
bool continuousHum = false;
bool continuousPH = false;
bool continuousWeight = false;

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
    Serial.println("start-temp-continuous, stop-temp-continuous");
    Serial.println("start-hum-continuous, stop-hum-continuous");
    Serial.println("start-ph-continuous, stop-ph-continuous");
    Serial.println("start-weight-continuous, stop-weight-continuous");
}

void loop() {
    // Continuous sensor readings based on individual flags
    static unsigned long lastRead = 0;
    if (millis() - lastRead > 1000) {  // Read every 1 second
        if (continuousTemp) {
            float temp = getDHTTemperature(false);
            Serial.print("Temp: ");
            Serial.print(temp);
            Serial.println(" °C");
            setLCDText("Temp: " + String(temp) + "C", 0, 0);
        }
        if (continuousHum) {
            float hum = getDHTHumidity();
            Serial.print("Humidity: ");
            Serial.print(hum);
            Serial.println(" %");
            setLCDText("Hum: " + String(hum) + "%", 0, 1);
        }
        if (continuousPH) {
            float ph = getPHValue();
            Serial.print("pH: ");
            Serial.println(ph);
            setLCDText("pH: " + String(ph), 0, 0);
        }
        if (continuousWeight) {
            float weight = getLOADCELLWeight();
            Serial.print("Weight: ");
            Serial.print(weight);
            Serial.println(" units");
            setLCDText("Weight: " + String(weight), 0, 1);
        }
        lastRead = millis();
    }

    // Standby mode display when no continuous monitoring is active
    static bool standbyShown = false;
    if (!continuousTemp && !continuousHum && !continuousPH && !continuousWeight) {
        if (!standbyShown) {
            setLCDText("Standby Mode", 0, 0);
            clearLCD();
            setLCDText("Standby Mode", 0, 0);
            standbyShown = true;
        }
    } else {
        standbyShown = false; // Reset when any continuous mode is active
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

        // Continuous monitoring commands
        else if (command == "start-temp-continuous") {
            continuousTemp = true;
            Serial.println("Temperature continuous monitoring started");
        }
        else if (command == "stop-temp-continuous") {
            continuousTemp = false;
            Serial.println("Temperature continuous monitoring stopped");
        }
        else if (command == "start-hum-continuous") {
            continuousHum = true;
            Serial.println("Humidity continuous monitoring started");
        }
        else if (command == "stop-hum-continuous") {
            continuousHum = false;
            Serial.println("Humidity continuous monitoring stopped");
        }
        else if (command == "start-ph-continuous") {
            continuousPH = true;
            Serial.println("pH continuous monitoring started");
        }
        else if (command == "stop-ph-continuous") {
            continuousPH = false;
            Serial.println("pH continuous monitoring stopped");
        }
        else if (command == "start-weight-continuous") {
            continuousWeight = true;
            Serial.println("Weight continuous monitoring started");
        }
        else if (command == "stop-weight-continuous") {
            continuousWeight = false;
            Serial.println("Weight continuous monitoring stopped");
        }

        // Process other commands and show results
        else if (command == "get-temp") {
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