/*
 * Smart Mixer - Eggshell Calcium Extraction System
 * 
 * Process Flow:
 * 1. Grinding Stage - Grind eggshells to target weight
 * 2. Vinegar Dispensing - Add vinegar at 1:12 ratio (weight-based)
 * 3. Reaction Monitoring - Monitor temp, pH, log data
 * 4. Completion Detection - Temp stabilizes, pH stabilizes
 * 
 * Features:
 * - Weight-based control for precise vinegar dosing
 * - Automatic calibration for load cell
 * - CSV data logging to SD card
 * - Temperature and pH monitoring
 */

#include "DHT_CONFIG.h"
#include "LCD_CONFIG.h"
#include "RELAY_CONFIG.h"
#include "HX711_CONFIG.h"
#include "PH_CONFIG.h"
#include "SD_CONFIG.h"
#include "BUTTON_CONFIG.h"

// Process states
enum ProcessState {
  STATE_IDLE,
  STATE_CALIBRATION,
  STATE_INPUT_TARGET,
  STATE_GRINDING,
  STATE_DISPENSING,
  STATE_MONITORING,
  STATE_COMPLETE,
  STATE_ERROR
};

// Global variables
ProcessState currentState = STATE_IDLE;
float targetEggshellGrams = 50.0;  // Default target
float vinegarRatio = 12.0;         // 1:12 ratio
float targetVinegarGrams = 0;
float currentWeight = 0;
float currentTemp = 0;
float currentPH = 0;
float initialWeightBeforeDispensing = 0;

// Reaction monitoring variables
float baselineTemp = 25.0;         // Ambient temperature
float tempThreshold = 5.0;         // Reaction active if temp > baseline + threshold
float phStableThreshold = 0.1;     // pH change threshold for stability
float lastPH = 0;
int phStableCount = 0;
const int phStableRequired = 10;   // Number of stable readings needed

// Timing variables
unsigned long processStartTime = 0;
unsigned long lastSensorRead = 0;
unsigned long lastDataLog = 0;
bool emergencyStop = false;

// Manual test controls
bool dhtTestActive = false;
bool loadcellTestActive = false;
bool lcdTestActive = false;
bool phTestActive = false;
bool sdTestActive = false;
bool lcdInitialized = false;
unsigned long lastManualTestRun = 0;
unsigned long lastSDTestRun = 0;
const unsigned long MANUAL_TEST_INTERVAL = 1000;
const unsigned long SD_TEST_INTERVAL = 5000;
const String sdTestFilename = "sd_test.txt";

// CSV filename
String csvFilename = "data.csv";

//-----------------------------------------------------------------
//SETUP FUNCTION---------------------------------------------------
//-----------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("========================================");
  Serial.println("Smart Mixer - Calcium Extraction System");
  Serial.println("========================================");
  
  // Initialize all modules
  initDHT();
  // initLCD();  // Uncomment if using LCD
  initRELAY();
  initLOADCELL();
  initPH();
  initSD();
  initBUTTONS();
  
  // Create CSV file for data logging
  createCSVFile(csvFilename);
  
  // Get baseline temperature from DHT22
  delay(2000);
  baselineTemp = getDHTTemperature(false);
  if (baselineTemp < 0) baselineTemp = 25.0;  // Default if sensor error
  
  Serial.print("Baseline Temperature: ");
  Serial.print(baselineTemp);
  Serial.println(" C");
  
  Serial.println("System Ready!");
  Serial.println("Press START button to begin");
  Serial.println("Press CALIBRATE button for calibration mode");
  Serial.println("Manual commands: relay1:on/off, relay2:on/off, dht:1/0, loadcell:1/0, lcd:1/0, ph:1/0, sd:1/0");
  
  currentState = STATE_IDLE;
}

//-----------------------------------------------------------------
//MAIN LOOP--------------------------------------------------------
//-----------------------------------------------------------------
void loop() {
  handleSerialCommands();

  // Read buttons
  setInputFlags();
  
  // Check for emergency stop
  if (inputFlags[1]) {  // STOP button
    emergencyStop = true;
    emergencyStopProcedure();
    inputFlags[1] = LOW;
  }
  
  // Check for calibration mode
  if (inputFlags[2] && currentState == STATE_IDLE) {  // CALIBRATE button
    currentState = STATE_CALIBRATION;
    inputFlags[2] = LOW;
  }
  
  // State machine
  switch (currentState) {
    case STATE_IDLE:
      handleIdleState();
      break;
      
    case STATE_CALIBRATION:
      handleCalibrationState();
      break;
      
    case STATE_INPUT_TARGET:
      handleInputTargetState();
      break;
      
    case STATE_GRINDING:
      handleGrindingState();
      break;
      
    case STATE_DISPENSING:
      handleDispensingState();
      break;
      
    case STATE_MONITORING:
      handleMonitoringState();
      break;
      
    case STATE_COMPLETE:
      handleCompleteState();
      break;
      
    case STATE_ERROR:
      handleErrorState();
      break;
  }

  runManualTests();
  
  // Small delay to prevent overwhelming the system
  delay(100);
}

//-----------------------------------------------------------------
//MANUAL SERIAL COMMAND HANDLER-----------------------------------
//-----------------------------------------------------------------
void handleSerialCommands() {
  if (Serial.available() == 0) return;

  String command = Serial.readStringUntil('\n');
  command.trim();
  command.toLowerCase();
  if (command.length() == 0) return;

  if (command == "relay1:on") {
    operateGRINDER(true);
    Serial.println("[Manual] Relay1 ON (grinder)");
  } else if (command == "relay1:off") {
    operateGRINDER(false);
    Serial.println("[Manual] Relay1 OFF (grinder)");
  } else if (command == "relay2:on") {
    operatePUMP(true);
    Serial.println("[Manual] Relay2 ON (pump)");
  } else if (command == "relay2:off") {
    operatePUMP(false);
    Serial.println("[Manual] Relay2 OFF (pump)");
  } else if (command == "dht:1") {
    dhtTestActive = true;
    Serial.println("[Manual] DHT22 test started");
  } else if (command == "dht:0") {
    dhtTestActive = false;
    Serial.println("[Manual] DHT22 test stopped");
  } else if (command == "loadcell:1") {
    loadcellTestActive = true;
    Serial.println("[Manual] Load cell test started");
  } else if (command == "loadcell:0") {
    loadcellTestActive = false;
    Serial.println("[Manual] Load cell test stopped");
  } else if (command == "lcd:1") {
    lcdTestActive = true;
    if (!lcdInitialized) {
      initLCD();
      lcdInitialized = true;
    }
    Serial.println("[Manual] LCD test started");
  } else if (command == "lcd:0") {
    lcdTestActive = false;
    Serial.println("[Manual] LCD test stopped");
  } else if (command == "ph:1") {
    phTestActive = true;
    Serial.println("[Manual] pH test started");
  } else if (command == "ph:0") {
    phTestActive = false;
    Serial.println("[Manual] pH test stopped");
  } else if (command == "sd:1") {
    sdTestActive = true;
    Serial.println("[Manual] SD card test started");
  } else if (command == "sd:0") {
    sdTestActive = false;
    Serial.println("[Manual] SD card test stopped");
  } else {
    Serial.print("[Manual] Unknown command: ");
    Serial.println(command);
  }
}

//-----------------------------------------------------------------
//MANUAL TEST EXECUTION-------------------------------------------
//-----------------------------------------------------------------
void runManualTests() {
  unsigned long now = millis();
  bool shouldRunSensors = (dhtTestActive || loadcellTestActive || lcdTestActive || phTestActive) && (now - lastManualTestRun >= MANUAL_TEST_INTERVAL);

  if (shouldRunSensors) {
    lastManualTestRun = now;

    if (dhtTestActive) {
      float temp = getDHTTemperature(false);
      float humidity = getDHTHumidity();
      Serial.print("[DHT] Temp: ");
      Serial.print(temp);
      Serial.print(" C, Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");
    }

    if (loadcellTestActive) {
      float weight = getLOADCELLWeight();
      Serial.print("[LOADCELL] Weight: ");
      Serial.print(weight);
      Serial.println(" g");
    }

    if (phTestActive) {
      float ph = getPHValue();
      Serial.print("[PH] Value: ");
      Serial.println(ph);
    }

    if (lcdTestActive) {
      if (!lcdInitialized) {
        initLCD();
        lcdInitialized = true;
      }
      clearLCD();
      setLCDText("LCD TEST", 0, 0);
      setLCDText("T:" , 0, 1);
      setLCDText(getDHTTemperature(false), 2, 1);
      setLCDText("W:" , 0, 2);
      setLCDText(getLOADCELLWeight(), 2, 2);
      setLCDText("PH:", 0, 3);
      setLCDText(getPHValue(), 3, 3);
    }
  }

  if (sdTestActive && (now - lastSDTestRun >= SD_TEST_INTERVAL)) {
    lastSDTestRun = now;
    String payload = "SD test at " + getTimestamp();
    writeToSD(sdTestFilename, payload);
  }
}

//-----------------------------------------------------------------
//STATE: IDLE------------------------------------------------------
//-----------------------------------------------------------------
void handleIdleState() {
  if (inputFlags[0]) {  // START button
    Serial.println("\n=== Starting Process ===");
    currentState = STATE_INPUT_TARGET;
    inputFlags[0] = LOW;
  }
}

//-----------------------------------------------------------------
//STATE: CALIBRATION-----------------------------------------------
//-----------------------------------------------------------------
void handleCalibrationState() {
  Serial.println("\n=== CALIBRATION MODE ===");
  Serial.println("Calibrating Load Cell...");
  
  // Load cell calibration with known weight
  Serial.println("Place empty container and press any key...");
  while (Serial.available() == 0) { delay(100); }
  Serial.read();
  
  tareLOADCELL();
  
  Serial.println("Place 100g weight and press any key...");
  while (Serial.available() == 0) { delay(100); }
  Serial.read();
  
  calibrateLOADCELL(100.0);
  
  Serial.println("\nCalibration Complete!");
  Serial.print("Load Cell Factor: ");
  Serial.println(getCalibrationFactor());
  
  currentState = STATE_IDLE;
}

//-----------------------------------------------------------------
//STATE: INPUT TARGET----------------------------------------------
//-----------------------------------------------------------------
void handleInputTargetState() {
  Serial.println("\n=== Input Target Weight ===");
  Serial.print("Enter target eggshell weight in grams (default 50g): ");
  
  // Wait for serial input with timeout
  unsigned long startWait = millis();
  String input = "";
  
  while (millis() - startWait < 10000) {  // 10 second timeout
    if (Serial.available() > 0) {
      input = Serial.readStringUntil('\n');
      float parsedValue = input.toFloat();
      if (parsedValue > 0 && parsedValue <= 1000) {
        targetEggshellGrams = parsedValue;
      }
      break;
    }
  }
  
  Serial.println(targetEggshellGrams);
  targetVinegarGrams = targetEggshellGrams * vinegarRatio;
  
  Serial.print("Target Eggshell: ");
  Serial.print(targetEggshellGrams);
  Serial.println(" g");
  Serial.print("Required Vinegar: ");
  Serial.print(targetVinegarGrams);
  Serial.println(" g");
  
  // Tare the scale
  tareLOADCELL();
  
  currentState = STATE_GRINDING;
}

//-----------------------------------------------------------------
//STATE: GRINDING--------------------------------------------------
//-----------------------------------------------------------------
void handleGrindingState() {
  static bool grindingStarted = false;
  
  if (!grindingStarted) {
    Serial.println("\n=== Grinding Stage ===");
    Serial.println("Place eggshells in grinder hopper");
    Serial.println("Starting grinder...");
    operateGRINDER(true);
    grindingStarted = true;
  }
  
  // Read current weight
  currentWeight = getLOADCELLWeight();
  
  // Display progress
  if (millis() - lastSensorRead >= 1000) {
    Serial.print("Current weight: ");
    Serial.print(currentWeight);
    Serial.print(" g / ");
    Serial.print(targetEggshellGrams);
    Serial.println(" g");
    lastSensorRead = millis();
  }
  
  // Check if target reached
  if (currentWeight >= targetEggshellGrams) {
    operateGRINDER(false);
    Serial.println("Target weight reached!");
    Serial.print("Final weight: ");
    Serial.print(currentWeight);
    Serial.println(" g");
    
    grindingStarted = false;
    currentState = STATE_DISPENSING;
  }
  
  // Safety check - if weight exceeds target by too much, stop
  if (currentWeight > targetEggshellGrams * 1.1) {
    operateGRINDER(false);
    Serial.println("Warning: Exceeded target weight!");
    grindingStarted = false;
    currentState = STATE_DISPENSING;
  }
}

//-----------------------------------------------------------------
//STATE: DISPENSING------------------------------------------------
//-----------------------------------------------------------------
void handleDispensingState() {
  static bool dispensingStarted = false;
  static float initialWeight = 0;
  static float targetTotalWeight = 0;
  
  if (!dispensingStarted) {
    Serial.println("\n=== Vinegar Dispensing Stage ===");
    Serial.print("Dispensing ");
    Serial.print(targetVinegarGrams);
    Serial.println(" g of vinegar...");
    
    // Record initial weight (eggshells + container)
    initialWeight = getLOADCELLWeight();
    initialWeightBeforeDispensing = initialWeight;
    targetTotalWeight = initialWeight + targetVinegarGrams;
    
    // Start dispensing
    operatePUMP(true);
    dispensingStarted = true;
  }
  
  // Read current weight
  currentWeight = getLOADCELLWeight();
  float dispensedWeight = currentWeight - initialWeight;
  
  // Check if target reached
  if (dispensedWeight >= targetVinegarGrams) {
    operatePUMP(false);
    Serial.println("Vinegar dispensing complete!");
    Serial.print("Final weight: ");
    Serial.print(currentWeight);
    Serial.println(" g");
    Serial.print("Dispensed: ");
    Serial.print(dispensedWeight);
    Serial.println(" g");
    
    dispensingStarted = false;
    processStartTime = millis();
    currentState = STATE_MONITORING;
  }
  
  // Display progress
  if (millis() - lastSensorRead >= 1000) {
    Serial.print("Current weight: ");
    Serial.print(currentWeight);
    Serial.print(" g (Dispensed: ");
    Serial.print(dispensedWeight);
    Serial.print(" g / ");
    Serial.print(targetVinegarGrams);
    Serial.println(" g)");
    lastSensorRead = millis();
  }
}

//-----------------------------------------------------------------
//STATE: MONITORING------------------------------------------------
//-----------------------------------------------------------------
void handleMonitoringState() {
  static bool monitoringStarted = false;
  
  if (!monitoringStarted) {
    Serial.println("\n=== Reaction Monitoring Stage ===");
    Serial.println("Monitoring temperature and pH...");
    lastPH = getPHValue();
    monitoringStarted = true;
  }
  
  // Read sensors periodically
  if (millis() - lastSensorRead >= SENSOR_READ_INTERVAL) {
    currentTemp = getDHTTemperature(false);
    currentPH = getPHValue();
    currentWeight = getLOADCELLWeight();
    
    // Log data to SD card
    unsigned long elapsedTime = millis() - processStartTime;
    float dispensedVinegarWeight = currentWeight - initialWeightBeforeDispensing;
    logDataToCSV(csvFilename, elapsedTime, currentTemp, currentPH, currentWeight, dispensedVinegarWeight);
    
    // Check reaction status
    float tempDiff = currentTemp - baselineTemp;
    float phChange = abs(currentPH - lastPH);
    
    Serial.println("--- Status ---");
    Serial.print("Time: ");
    Serial.print(elapsedTime / 1000);
    Serial.println(" s");
    Serial.print("Temp: ");
    Serial.print(currentTemp);
    Serial.print(" C (Δ");
    Serial.print(tempDiff);
    Serial.println(" C)");
    Serial.print("pH: ");
    Serial.print(currentPH);
    Serial.print(" (Δ");
    Serial.print(phChange);
    Serial.println(")");
    
    // Check for reaction completion
    if (tempDiff < tempThreshold && phChange < phStableThreshold) {
      phStableCount++;
      Serial.print("Stability count: ");
      Serial.print(phStableCount);
      Serial.print(" / ");
      Serial.println(phStableRequired);
    } else {
      phStableCount = 0;
      Serial.println("Reaction active...");
    }
    
    // Completion condition: temp back to baseline, pH stable
    if (phStableCount >= phStableRequired) {
      Serial.println("\n=== Reaction Complete! ===");
      Serial.println("Temperature and pH stabilized");
      monitoringStarted = false;
      currentState = STATE_COMPLETE;
    }
    
    // Safety timeout (3 hours)
    if (elapsedTime >= MAX_REACTION_TIME) {
      Serial.println("\n=== Maximum time reached ===");
      monitoringStarted = false;
      currentState = STATE_COMPLETE;
    }
    
    lastPH = currentPH;
    lastSensorRead = millis();
  }
}

//-----------------------------------------------------------------
//STATE: COMPLETE--------------------------------------------------
//-----------------------------------------------------------------
void handleCompleteState() {
  Serial.println("\n========================================");
  Serial.println("Process Complete!");
  Serial.println("========================================");
  Serial.print("Total weight: ");
  Serial.print(currentWeight);
  Serial.println(" g");
  Serial.print("Total vinegar: ");
  Serial.print(currentWeight - initialWeightBeforeDispensing);
  Serial.println(" g");
  Serial.print("Final temp: ");
  Serial.print(currentTemp);
  Serial.println(" C");
  Serial.print("Final pH: ");
  Serial.println(currentPH);
  Serial.println("Data saved to SD card: " + csvFilename);
  Serial.println("\nPress START to begin new process");
  
  currentState = STATE_IDLE;
}

//-----------------------------------------------------------------
//STATE: ERROR-----------------------------------------------------
//-----------------------------------------------------------------
void handleErrorState() {
  Serial.println("\n!!! ERROR STATE !!!");
  Serial.println("System halted. Please check and restart.");
  
  // Turn off all actuators
  operateGRINDER(false);
  operatePUMP(false);
  
  while (true) {
    delay(1000);
  }
}

//-----------------------------------------------------------------
//EMERGENCY STOP PROCEDURE-----------------------------------------
//-----------------------------------------------------------------
void emergencyStopProcedure() {
  Serial.println("\n!!! EMERGENCY STOP !!!");
  
  // Immediately stop all motors
  operateGRINDER(false);
  operatePUMP(false);
  
  // Log final state
  Serial.println("System stopped by user");
  Serial.print("Current state was: ");
  Serial.println(currentState);
  
  // Reset to idle
  currentState = STATE_IDLE;
  emergencyStop = false;
  
  Serial.println("Press START to restart process");
}