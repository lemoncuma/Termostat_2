#ifndef CONFIG_H
#define CONFIG_H

// Pin definitions
#if defined(ESP8266_PLATFORM)
  // ESP8266 pin definitions
  #define DHT_PIN         D4     // DHT21 data pin
  #define RELAY_HEAT_PIN  D1     // Heat relay pin
  #define RELAY_COOL_PIN  D2     // Cool relay pin
  #define STATUS_LED_PIN  D5     // Status LED pin
  #define HEAT_LED_PIN    D6     // Heating active LED pin
  #define COOL_LED_PIN    D7     // Cooling active LED pin
  #define ACTIVE_SWITCH_PIN D3   // Active status switch pin
  #define FAN_PIN         D0     // PWM pin for fan control
#else
  // ESP32 pin definitions
  #define DHT_PIN         4      // DHT21 data pin
  #define RELAY_HEAT_PIN  16     // Heat relay pin
  #define RELAY_COOL_PIN  17     // Cool relay pin
  #define STATUS_LED_PIN  5      // Status LED pin
  #define HEAT_LED_PIN    18     // Heating active LED pin
  #define COOL_LED_PIN    19     // Cooling active LED pin
  #define ACTIVE_SWITCH_PIN 21   // Active status switch pin
  #define FAN_PIN         23     // PWM pin for fan control
#endif

// Default values
#define DEFAULT_TEMP      21.0   // Default temperature set point in Celsius
#define TEMP_THRESHOLD    0.5    // Temperature threshold for activating heating/cooling
#define DHT_TYPE          DHT21  // DHT sensor type (DHT11, DHT21, DHT22)
#define SENSOR_READ_INTERVAL 5000 // Read sensor every 5 seconds
#define HOMEKIT_SETUP_CODE "466-37-726" // HomeKit pairing code

// WiFi settings
#define WIFI_SSID "Mas_IoT"   // WiFi SSID
#define WIFI_PASSWORD "Mas.799307" // WiFi password (minimum 8 characters)

// System operation modes
enum OperationMode {
  OFF = 0,
  HEAT = 1,
  COOL = 2,
  AUTO = 3
};

#endif // CONFIG_H