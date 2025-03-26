# HomeKit Compatible Thermostat using ESP8266/ESP32

This project implements a HomeKit-compatible thermostat using ESP8266 or ESP32 microcontrollers. It allows you to control your heating and cooling systems through Apple HomeKit.

## Features

- Temperature and humidity sensing using DHT21 sensor
- HomeKit integration for controlling the thermostat from Apple devices
- Support for heating and cooling modes
- LED indicators for system status
- Status switch that activates when heating or cooling is running
- Simple setup and configuration

## Hardware Requirements

- ESP8266 (NodeMCU) or ESP32 development board
- DHT21 temperature and humidity sensor
- 2 Relay modules (for heating and cooling control)
- 3 LEDs (status, heating, cooling)
- 3 220-330 ohm resistors for LEDs
- 1 external switch/relay for active status indication
- Breadboard and connecting wires
- 5V power supply

## Wiring Diagram

### ESP8266 (NodeMCU)
- DHT21 data pin → D4
- Heating relay pin → D1
- Cooling relay pin → D2
- Status LED pin → D5
- Heating LED pin → D6
- Cooling LED pin → D7
- Active status switch pin → D3

### ESP32
- DHT21 data pin → GPIO4
- Heating relay pin → GPIO16
- Cooling relay pin → GPIO17
- Status LED pin → GPIO5
- Heating LED pin → GPIO18
- Cooling LED pin → GPIO19
- Active status switch pin → GPIO21

## Software Setup

### Prerequisites
- PlatformIO installed (recommended) or Arduino IDE
- Required libraries:
  - HomeSpan
  - DHT sensor library
  - Adafruit Unified Sensor

### Building and Uploading

#### Using PlatformIO
1. Clone the repository
2. Open the project in PlatformIO
3. Select your board (ESP8266 or ESP32)
4. Build and upload the project

#### Using Arduino IDE
1. Install the required libraries from the Library Manager
2. Open the main.cpp file and rename it to main.ino
3. Select your board from the Tools menu
4. Upload the sketch

## HomeKit Setup

1. Power on your ESP device
2. Open the Home app on your iOS device
3. Tap "Add Accessory"
4. Scan the QR code or select "I Don't Have a Code"
5. Select the "HomeKit Thermostat" from the list
6. Enter the pairing code: 466-37-726
7. Complete the setup by following the in-app instructions

## Usage

Once set up, you can control your thermostat from the Apple Home app:
- Set the target temperature
- Switch between Off, Heat, Cool, and Auto modes
- View the current temperature and humidity

## Customization

You can customize the thermostat by modifying the `config.h` file:
- Change pin assignments
- Adjust temperature thresholds
- Modify the default temperature
- Change the HomeKit setup code
- Configure WiFi settings (SSID and password)

## Troubleshooting

- If the device isn't detected in HomeKit, try resetting it and rechecking your network connection
- Verify that all connections are secure
- Check the serial monitor for debugging information

## License

This project is open source and available under the MIT License.
