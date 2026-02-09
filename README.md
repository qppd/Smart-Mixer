# Microcontroller-Regulated Eggshell Grinder and Reactor for Efficient Calcium Acetate Fertilizer Synthesis 🌱⚗️

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino](https://img.shields.io/badge/Arduino-1.8.19%2B-blue)](https://www.arduino.cc/)

An automated Arduino-based system that grinds eggshells to precise weights and reacts them with vinegar to produce calcium acetate fertilizer, with intelligent monitoring and control throughout the entire synthesis process.

## Table of Contents

- [About](#-about)
- [Features](#-features)
- [How It Works](#-how-it-works)
- [Hardware Requirements](#-hardware-requirements)
- [Software Requirements](#-software-requirements)
- [Installation](#-installation)
- [Usage](#-usage)
- [Project Structure](#-project-structure)
- [Configuration](#-configuration)
- [API Reference](#-api-reference)
- [Contributing](#-contributing)
- [License](#-license)
- [Author](#-author)

## About

This project implements a complete automated synthesis system for calcium acetate fertilizer production. The microcontroller regulates an eggshell grinding process to achieve precise weight measurements, followed by automated vinegar addition in optimal ratios, and continuous monitoring of the exothermic reaction until completion.

**Chemical Reaction**: CaCO₃ (egg shells) + 2CH₃COOH (vinegar) → Ca(CH₃COO)₂ (calcium acetate) + H₂O + CO₂

**Key Features**:
- Precision grinding with real-time weight feedback
- Automated 1:12 ratio vinegar addition
- Reaction monitoring until bubbling cessation
- Temperature stabilization at ~39°C
- Complete process automation with LCD user interface

## Features

### Automated Fertilizer Synthesis
- **Precision Grinding**: Automated eggshell grinding to exact weight specifications
- **Intelligent Ratio Control**: Automated 1:12 eggshell-to-vinegar ratio addition
- **Reaction Monitoring**: Continuous tracking until bubbling stops and temperature stabilizes
- **Complete Process Automation**: From grinding to final product with minimal user intervention

### Real-time Monitoring
- **Temperature Sensing**: DHT22 sensor tracking reaction temperature (target: ~39°C)
- **Weight Measurement**: HX711 load cell for precise grinding and ratio control
- **pH Level Detection**: Analog pH sensor monitoring reaction progress
- **Serial Output**: Real-time data logging and display

### Control Systems
- **Grinding Motor Control**: SSR-controlled motor for eggshell grinding
- **Automated Pumping**: Relay-controlled pump for precise vinegar addition
- **LCD User Interface**: I2C LCD for process status and user input
- **Button Input System**: User controls for process initiation and parameter input
- **Dual Relay Control**: SSR and regular relays for mixing operations
- **LCD Display**: I2C LCD for local status visualization (configurable)
- **Power Management**: Sleep/wake functionality for energy efficiency

###  Software Architecture
- **Modular Design**: Separate configuration files for each component
- **Centralized Pin Management**: All hardware pins defined in one location
- **Extensible Codebase**: Easy to add new sensors or actuators

##  How It Works

### Complete Synthesis Process Flowchart

```
START
  ↓
Put Eggshell → Press Start Button → LCD Shows Message
  ↓
Input Desired Grams (g) to Grind → Loadcell Ready
  ↓
Motor ON (Grinding Starts)
  ↓
┌─ Decision: Loadcell = Desired Weight?
│   NO → Continue Grinding (Motor ON)
└─ YES → Motor OFF → Pump ON
      ↓
Pump Runs Until 1:12 Ratio Achieved
      ↓
Monitor Solution (Temperature & pH)
      ↓
Store Data & Continue Monitoring
      ↓
Until Bubbling Stops & Temp ≈ 39°C
      ↓
END (Fertilizer Ready)
```

### Detailed Process Steps

1. **System Initialization**: Arduino boots up, initializes sensors and LCD
2. **Eggshell Loading**: User places eggshells in grinding chamber
3. **Process Start**: User presses button to begin synthesis
4. **Parameter Input**: LCD prompts for desired eggshell weight in grams
5. **Grinding Phase**: Motor activates, load cell monitors weight in real-time
6. **Weight Verification**: System checks if target weight is achieved
7. **Grinding Completion**: Motor stops when desired weight is reached
8. **Vinegar Addition**: Pump activates to add vinegar in 1:2 ratio
9. **Reaction Monitoring**: Continuous temperature and pH monitoring begins
10. **Data Logging**: All sensor readings stored and displayed
11. **Completion Detection**: Process continues until bubbling stops and temperature stabilizes at ~39°C
12. **System Shutdown**: Automatic completion when reaction is finished

### Control Logic
- **Grinding Control**: SSR Relay (Pin 23) controls grinding motor with weight feedback
- **Pumping Control**: Regular Relay (Pin 22) controls vinegar pump with ratio feedback
- **Reaction Endpoints**: Process completes when CO₂ evolution ceases and temperature peaks
- **Safety Monitoring**: Continuous parameter checking prevents unsafe conditions

## Hardware Requirements

### Core Components
- **Arduino Board** (Uno, Mega, or compatible)
- **DHT22 Temperature & Humidity Sensor**
- **HX711 Load Cell Amplifier** + Load Cell (for weight measurement)
- **Analog pH Sensor Module**
- **I2C LCD Display (16x2 or 20x4)**
- **Momentary Push Button** (for process initiation)
- **DC Motor** (for eggshell grinding)
- **Peristaltic Pump** (for vinegar addition)
- **2-Channel Relay Module** (Regular + SSR for motor/pump control)

### Mechanical Components
- **Grinding Chamber**: Container for eggshell grinding
- **Reaction Vessel**: Container for fertilizer synthesis
- **Load Cell Mounting**: Proper setup for weight measurement
- **Tubing System**: For vinegar transfer from pump to reaction vessel

### Pin Configuration
```
DHT22 Sensor      -> Pin 2
HX711 DT          -> Pin 4
HX711 SCK         -> Pin 3
pH Sensor         -> A0
LCD I2C           -> I2C pins (SDA, SCL)
Buttons           -> Pins 24 (Start), 5 (Stop), 6 (Calibrate)
Grinding Motor    -> Pin 23 (via SSR Relay)
Vinegar Pump      -> Pin 22 (via Regular Relay)
SD Card CS        -> Pin 53 (Arduino Mega)
```

### Wiring Diagram
![Wiring Diagram](wiring/SmartMixer.png)

The Fritzing file is available at [wiring/SmartMixer.fzz](wiring/SmartMixer.fzz)

## Software Requirements

### Arduino IDE
- **Version**: 1.8.19 or higher
- **Board**: Arduino Uno/Mega (or compatible)

### Required Libraries
Install via Arduino Library Manager:
- `DHT sensor library` by Adafruit
- `LiquidCrystal_I2C` by Marco Schwartz
- `HX711` by Bogdan Necula

## Installation

1. **Clone the Repository**
   ```bash
   git clone https://github.com/qppd/Smart-Mixer.git
   cd Smart-Mixer
   ```

2. **Open Arduino IDE**
   - Navigate to `src/arduino/SmartMixer/SmartMixer.ino`
   - Open the file in Arduino IDE

3. **Install Required Libraries**
   - Go to `Sketch > Include Library > Manage Libraries`
   - Search and install:
     - DHT sensor library
     - LiquidCrystal_I2C
     - HX711

4. **Configure Hardware**
   - Connect sensors according to pin configuration
   - Verify power supplies for all components

5. **Upload Code**
   - Select correct board and port
   - Click Upload button

## Usage

### Fertilizer Synthesis Process

1. **System Setup**: Power on Arduino, ensure all sensors are connected
2. **Load Eggshells**: Place eggshells in the grinding chamber on the load cell
3. **Start Process**: Press the start button to initiate synthesis
4. **Parameter Input**: LCD displays "Enter weight (g):", use buttons to input desired grams
5. **Grinding Phase**: Motor activates, grinding continues until target weight is reached
6. **Weight Verification**: System automatically stops grinding when load cell measures desired weight
7. **Vinegar Addition**: Pump activates to add vinegar in 1:12 ratio to eggshell weight
8. **Reaction Monitoring**: System continuously monitors temperature and pH
9. **Data Logging**: All readings displayed on Serial Monitor and optionally LCD
10. **Completion Detection**: Process ends when bubbling stops and temperature stabilizes at ~39°C

### Serial Output Format
```
Temperature: 39.20°C --- Weight: 150.00g --- pH: 4.80 --- Status: REACTING
Temperature: 39.50°C --- Weight: 450.00g --- pH: 5.20 --- Status: COMPLETE
```

### Process Parameters
- **Grinding Weight**: User-defined (typically 100-200g eggshells)
- **Vinegar Ratio**: Automatic 1:12 eggshell-to-vinegar ratio
- **Reaction Temperature**: Target ~39°C (normal temperature change)
- **Completion Criteria**: Bubbling cessation + temperature stabilization
- **Monitoring Duration**: Continuous until reaction completion

### Control Examples
```cpp
// Start grinding motor
operateSSR(RELAY_GRINDER, true);

// Check if target weight reached
if (getLOADCELLWeight() >= targetWeight) {
    operateSSR(RELAY_GRINDER, false); // Stop grinding
    operateRELAY(RELAY_PUMP, true);   // Start vinegar pump
}

// Monitor reaction completion
if (getPHValue() >= 4.5 && getDHTTemperature(false) >= 38.0) {
    // Reaction complete - stop all operations
    operateRELAY(RELAY_PUMP, false);
}
```

### LCD Display (Optional)
Uncomment LCD initialization in `setup()`:
```cpp
initLCD();
setLCDText("Fertilizer", 0, 0);
setLCDText("Production", 0, 1);
```

## Project Structure

```
Smart-Mixer/
├── LICENSE
├── README.md
└── src/
    └── arduino/
        └── SmartMixer/
            ├── SmartMixer.ino          # Main Arduino sketch
            ├── PINS_CONFIG.h           # Hardware pin definitions
            ├── DHT_CONFIG.h/.cpp       # Temperature sensor
            ├── HX711_CONFIG.h/.cpp     # Load cell amplifier
            ├── PH_CONFIG.h/.cpp        # pH sensor
            ├── LCD_CONFIG.h/.cpp       # LCD display
            └── RELAY_CONFIG.h/.cpp     # Relay control
               ├── SD_CONFIG.h/.cpp        # SD card logging
               ├── BUTTON_CONFIG.h/.cpp    # 3-button input handling
```

### File Descriptions

 - **`SD_CONFIG.*`**: SD card logging and storage
 - **`BUTTON_CONFIG.*`**: Three-button input handling with debounce

## Configuration

### Fertilizer Synthesis Parameters
- **Target Grinding Weight**: User-defined (100-200g recommended)
- **Vinegar Ratio**: 1:12 eggshell-to-vinegar (automatic calculation)
- **Reaction Temperature**: ~39°C (normal temperature increase)
- **Completion Criteria**: Bubbling cessation + temperature stabilization
- **Safety Limits**: Max 45°C, Min pH 4.0, Max weight 500g

### Calibration Values
- **HX711 Scale Factor**: `22500.3f` (adjust for your load cell)
- **pH Calibration**: `21.34` (calibrate with pH 4.0 and 7.0 buffers)

### Pin Customization
Edit `PINS_CONFIG.h` to modify hardware connections:
```cpp
#define TEMP_SENSOR_PIN 2  // Temperature sensor
#define HX711_DT 4         // Load cell data
#define HX711_SCK 3        // Load cell clock
#define PH_PIN A0          // pH sensor analog input
#define BUTTON_START 24    // Start button
#define BUTTON_STOP 5      // Stop button
#define BUTTON_CALIBRATE 6 // Calibrate button
#define SD_CS 53           // SD card chip select (Arduino Mega)
#define RELAY_GRINDER 23   // Grinding motor relay (SSR)
#define RELAY_PUMP 22      // Vinegar pump relay (regular)
```

### Sensor Parameters
- **DHT Type**: Currently set to DHT22 (configurable)
- **pH Sampling**: 10 readings with median filtering for noise reduction
- **Weight Averaging**: 10-sample moving average for stability
- **Button Debouncing**: 50ms debounce delay
- **LCD Refresh Rate**: 500ms update interval

## API Reference

### Temperature Functions
```cpp
void initDHT();                           // Initialize DHT sensor
float getDHTTemperature(boolean fahrenheit); // Get temperature
float getDHTHumidity();                   // Get humidity
```

### Weight Functions
```cpp
void initLOADCELL();                      // Initialize HX711
float getLOADCELLWeight();                // Get weight reading
void operateHX711sleep();                 // Put HX711 to sleep
void operateHX711wake();                  // Wake HX711
```

### pH Functions
```cpp
float getPHValue();                       // Get filtered pH reading
```

### LCD Functions
```cpp
void initLCD();                           // Initialize LCD
void clearLCD();                          // Clear display
void setLCDText(String text, int x, int y); // Display text
void setLCDText(float value, int x, int y); // Display number
```

### Relay Functions
```cpp
void initRELAY();                         // Initialize relays
void operateRELAY(uint16_t relay, boolean state); // Control regular relay
void operateSSR(uint16_t relay, boolean state);   // Control SSR
```

## Contributing

We welcome contributions! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Development Guidelines
- Follow Arduino coding standards
- Add comments to complex logic
- Test hardware compatibility
- Update documentation

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👤 Author

**Sajed Lopez Mendoza**
- GitHub: [@qppd](https://github.com/qppd)
- Project: [Smart-Mixer](https://github.com/qppd/Smart-Mixer)

---

⭐ **Star this repository** if you find it useful!

For questions or support, please open an issue on GitHub.