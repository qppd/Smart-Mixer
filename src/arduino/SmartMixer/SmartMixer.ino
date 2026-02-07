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
    Serial.println("get-temp (toggle continuous), get-hum (toggle continuous)");
    Serial.println("get-ph (toggle continuous), get-weight (toggle continuous)");
    Serial.println("relay1_on, relay1_off, relay2_on, relay2_off");
    Serial.println("set-lcd:<text>, stop");
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
            continuousTemp = !continuousTemp; // Toggle continuous mode
            if (continuousTemp) {
                Serial.println("Temperature continuous monitoring started");
                setLCDText("Temp Cont: ON", 0, 0);
            } else {
                Serial.println("Temperature continuous monitoring stopped");
                setLCDText("Temp Cont: OFF", 0, 0);
            }
            // Also show current reading
            float temp = getDHTTemperature(false);
            Serial.print("Current Temp: ");
            Serial.print(temp);
            Serial.println(" °C");
        }
        else if (command == "get-hum") {
            continuousHum = !continuousHum; // Toggle continuous mode
            if (continuousHum) {
                Serial.println("Humidity continuous monitoring started");
                setLCDText("Hum Cont: ON", 0, 1);
            } else {
                Serial.println("Humidity continuous monitoring stopped");
                setLCDText("Hum Cont: OFF", 0, 1);
            }
            // Also show current reading
            float hum = getDHTHumidity();
            Serial.print("Current Hum: ");
            Serial.print(hum);
            Serial.println(" %");
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
            continuousPH = !continuousPH; // Toggle continuous mode
            if (continuousPH) {
                Serial.println("pH continuous monitoring started");
                setLCDText("pH Cont: ON", 0, 0);
            } else {
                Serial.println("pH continuous monitoring stopped");
                setLCDText("pH Cont: OFF", 0, 0);
            }
            // Also show current reading
            float ph = getPHValue();
            Serial.print("Current pH: ");
            Serial.println(ph);
        }
        else if (command == "get-weight") {
            continuousWeight = !continuousWeight; // Toggle continuous mode
            if (continuousWeight) {
                Serial.println("Weight continuous monitoring started");
                setLCDText("Weight Cont: ON", 0, 1);
            } else {
                Serial.println("Weight continuous monitoring stopped");
                setLCDText("Weight Cont: OFF", 0, 1);
            }
            // Also show current reading
            float weight = getLOADCELLWeight();
            Serial.print("Current Weight: ");
            Serial.print(weight);
            Serial.println(" units");
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