# Smart Mixer 🍹

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino](https://img.shields.io/badge/Arduino-1.8.19%2B-blue)](https://www.arduino.cc/)

A comprehensive IoT-enabled smart mixing system built with Arduino, featuring real-time monitoring of temperature, weight, and pH levels with automated control capabilities.

## 📋 Table of Contents

- [Features](#-features)
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

## ✨ Features

### 🔍 Real-time Monitoring
- **Temperature Sensing**: DHT22 sensor for accurate temperature readings
- **Weight Measurement**: HX711 load cell amplifier for precise weight monitoring
- **pH Level Detection**: Analog pH sensor with noise-filtered readings
- **Serial Output**: Real-time data display via Serial monitor

### 🎛️ Control Systems
- **Dual Relay Control**: Regular and solid-state relays for mixing operations
- **LCD Display**: I2C LCD for local status visualization (configurable)
- **Power Management**: Sleep/wake functionality for energy efficiency

### 🏗️ Software Architecture
- **Modular Design**: Separate configuration files for each component
- **Centralized Pin Management**: All hardware pins defined in one location
- **Extensible Codebase**: Easy to add new sensors or actuators

## 🔧 Hardware Requirements

### Core Components
- **Arduino Board** (Uno, Mega, or compatible)
- **DHT22 Temperature & Humidity Sensor**
- **HX711 Load Cell Amplifier** + Load Cell
- **Analog pH Sensor Module**
- **I2C LCD Display (16x2 or 20x4)**
- **2-Channel Relay Module** (Regular + SSR)

### Pin Configuration
```
DHT22 Sensor    -> Pin 2
HX711 DT        -> Pin 25
HX711 SCK       -> Pin 23
pH Sensor       -> A0
LCD I2C         -> I2C pins (SDA, SCL)
Relay 1 (REG)   -> Pin 22
Relay 2 (SSR)   -> Pin 23
```

## 💻 Software Requirements

### Arduino IDE
- **Version**: 1.8.19 or higher
- **Board**: Arduino Uno/Mega (or compatible)

### Required Libraries
Install via Arduino Library Manager:
- `DHT sensor library` by Adafruit
- `LiquidCrystal_I2C` by Marco Schwartz
- `HX711` by Bogdan Necula

## 🚀 Installation

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

## 📖 Usage

### Basic Operation
1. Power on the Arduino
2. Open Serial Monitor (9600 baud)
3. View real-time sensor data
4. Control relays programmatically

### Serial Output Format
```
Temperature: 25.50 --- Weight: 150.25 --- pH: 7.20 ---
```

### Relay Control Examples
```cpp
// Turn on regular relay
operateRELAY(RELAY_1, true);

// Turn off solid-state relay
operateSSR(RELAY_2, false);
```

### LCD Display (Optional)
Uncomment LCD initialization in `setup()`:
```cpp
initLCD();
setLCDText("Temp: ", 0, 0);
setLCDText(getDHTTemperature(false), 6, 0);
```

## 📁 Project Structure

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
```

### File Descriptions

- **`SmartMixer.ino`**: Main program loop and initialization
- **`PINS_CONFIG.h`**: Centralized hardware pin configuration
- **`DHT_CONFIG.*`**: Temperature and humidity sensing
- **`HX711_CONFIG.*`**: Weight measurement system
- **`PH_CONFIG.*`**: pH level monitoring with noise filtering
- **`LCD_CONFIG.*`**: I2C LCD display interface
- **`RELAY_CONFIG.*`**: Dual relay control system

## ⚙️ Configuration

### Calibration Values
- **HX711 Scale Factor**: `22500.3f` (adjust for your load cell)
- **pH Calibration**: `21.34` (calibrate with known pH solutions)

### Pin Customization
Edit `PINS_CONFIG.h` to modify hardware connections:
```cpp
#define DHT_PIN 2
#define HX711_DT 25
#define RELAY_1 22
// ... etc
```

### Sensor Parameters
- **DHT Type**: Currently set to DHT22 (configurable)
- **pH Sampling**: 10 readings with median filtering
- **Weight Averaging**: 10-sample moving average

## 📚 API Reference

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

## 🤝 Contributing

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

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👤 Author

**Sajed Lopez Mendoza**
- GitHub: [@qppd](https://github.com/qppd)
- Project: [Smart-Mixer](https://github.com/qppd/Smart-Mixer)

---

⭐ **Star this repository** if you find it useful!

For questions or support, please open an issue on GitHub.