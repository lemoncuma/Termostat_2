#include "thermostat.h"

// Temperature Sensor implementation
TemperatureSensor::TemperatureSensor(DHT *dhtSensor) : Service::TemperatureSensor() {
  dht = dhtSensor;
  currentTemp = new Characteristic::CurrentTemperature(20.0); // Default value
  currentTemp->setRange(-50, 100);  // Set a wider temperature range than default
}

void TemperatureSensor::updateTemperature(float temperature) {
  currentTemp->setVal(temperature);
}

// Humidity Sensor implementation
HumiditySensor::HumiditySensor(DHT *dhtSensor) : Service::HumiditySensor() {
  dht = dhtSensor;
  currentHumidity = new Characteristic::CurrentRelativeHumidity(50.0); // Default value
}

void HumiditySensor::updateHumidity(float humidity) {
  currentHumidity->setVal(humidity);
}

// Thermostat implementation
ThermostatService::ThermostatService(DHT *dhtSensor) : Service::HeaterCooler() {
  dht = dhtSensor;
  
  // Initialize characteristics for HeaterCooler service
  currentTemp = new Characteristic::CurrentTemperature(20.0);
  currentTemp->setRange(-50, 100);
  
  targetTemp = new Characteristic::HeatingThresholdTemperature(DEFAULT_TEMP);
  targetTemp->setRange(10, 30);  // Min 10°C, Max 30°C
  
  // Cooling threshold also needed for HeaterCooler service
  new Characteristic::CoolingThresholdTemperature(25.0);
  
  currentState = new Characteristic::CurrentHeaterCoolerState(0);  // 0=INACTIVE
  targetState = new Characteristic::TargetHeaterCoolerState(0);    // 0=AUTO, 1=HEAT, 2=COOL
  
  tempDisplayUnits = new Characteristic::TemperatureDisplayUnits(0); // 0=Celsius, 1=Fahrenheit
  
  // Required by HeaterCooler
  active = new Characteristic::Active(1);  // 1=ACTIVE by default
  
  // Fan hızı kontrolü - HeaterCooler için RotationSpeed kullanıyoruz
  rotationSpeed = new Characteristic::RotationSpeed(0);  // Default 0%
  rotationSpeed->setRange(0, 100);                       // 0-100% range
  
  tempSetPoint = DEFAULT_TEMP;
  currentMode = OFF;
  
  // Initialize relay pins
  pinMode(RELAY_HEAT_PIN, OUTPUT);
  pinMode(RELAY_COOL_PIN, OUTPUT);
  digitalWrite(RELAY_HEAT_PIN, LOW);  // Make sure relays start in OFF state
  digitalWrite(RELAY_COOL_PIN, LOW);
  
  // Initialize LED pins
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(HEAT_LED_PIN, OUTPUT);
  pinMode(COOL_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, HIGH); // Status LED on
  digitalWrite(HEAT_LED_PIN, LOW);    // Heating LED off
  digitalWrite(COOL_LED_PIN, LOW);    // Cooling LED off
  
  // Initialize active status switch
  pinMode(ACTIVE_SWITCH_PIN, OUTPUT);
  digitalWrite(ACTIVE_SWITCH_PIN, LOW); // Switch starts in OFF state
  
  // Initialize FAN PWM pin
  pinMode(FAN_PIN, OUTPUT);
  analogWrite(FAN_PIN, 0);  // Start with fan off
}

boolean ThermostatService::update() {
  // This is called when a HomeKit controller requests a change
  
  if (active->updated()) {
    int isActive = active->getNewVal();
    LOG1("HomeKit requested thermostat active state change to: ");
    LOG1(isActive ? "ACTIVE\n" : "INACTIVE\n");
    
    // If turned off, set the thermostat to OFF mode
    if (isActive == 0) {
      currentMode = OFF;
      // Turn off all relays
      digitalWrite(RELAY_HEAT_PIN, LOW);
      digitalWrite(RELAY_COOL_PIN, LOW);
      // Update state to inactive
      currentState->setVal(0); // INACTIVE
      // Turn off fan
      analogWrite(FAN_PIN, 0);
      rotationSpeed->setVal(0);
    }
    
    updateHeatingCooling();
    return true;
  }
  
  if (targetState->updated()) {
    int newMode = targetState->getNewVal();
    LOG1("HomeKit requested thermostat mode change to: ");
    
    switch (newMode) {
      case 0:
        LOG1("AUTO\n");
        currentMode = AUTO;
        break;
      case 1:
        LOG1("HEAT\n");
        currentMode = HEAT;
        break;
      case 2:
        LOG1("COOL\n");
        currentMode = COOL;
        break;
      default:
        LOG1("OFF\n");
        currentMode = OFF;
        break;
    }
    
    // Ensure active state is set to 1 when in any mode other than OFF
    if (currentMode != OFF && active->getVal() == 0) {
      active->setVal(1);
    }
    
    // Update the current state based on the target state
    updateHeatingCooling();
    return true;
  }
  
  if (targetTemp->updated()) {
    tempSetPoint = targetTemp->getNewVal<float>();
    LOG1("HomeKit requested temperature change to: ");
    LOG1(tempSetPoint);
    LOG1("°C\n");
    
    // Update heating/cooling based on the new target
    updateHeatingCooling();
    return true;
  }
  
  if (rotationSpeed->updated()) {
    int fanSpeed = rotationSpeed->getNewVal();
    LOG1("HomeKit requested fan speed change to: ");
    LOG1(fanSpeed);
    LOG1("%\n");
    
    // Set fan speed using PWM (0-255)
    int pwmValue = map(fanSpeed, 0, 100, 0, 255);
    analogWrite(FAN_PIN, pwmValue);
    
    return true;
  }
  
  return false;
}

void ThermostatService::loop() {
  // This is called periodically to update the thermostat state
  updateHeatingCooling();
  setLedIndicators();
}

void ThermostatService::updateHeatingCooling() {
  float currentTempVal = currentTemp->getVal<float>();
  // HeaterCooler CurrentHeaterCoolerState değerleri:
  // 0=INACTIVE, 1=IDLE, 2=HEATING, 3=COOLING
  int currentHeatingCoolingState = 0; // INACTIVE by default
  
  // Turn off both relays by default
  digitalWrite(RELAY_HEAT_PIN, LOW);
  digitalWrite(RELAY_COOL_PIN, LOW);
  
  // Check if the device is active
  int isActive = active->getVal();
  
  if (isActive == 1 && currentMode != OFF) {
    // Start with IDLE state if the system is on but not actively heating/cooling
    currentHeatingCoolingState = 1; // IDLE
    
    if (currentMode == HEAT || currentMode == AUTO) {
      // Check if we need heating
      if (currentTempVal < (tempSetPoint - TEMP_THRESHOLD)) {
        digitalWrite(RELAY_HEAT_PIN, HIGH);  // Turn on heating
        digitalWrite(RELAY_COOL_PIN, LOW);   // Ensure cooling is off
        currentHeatingCoolingState = 2;      // HEATING
      }
    }
    
    if (currentMode == COOL || currentMode == AUTO) {
      // Check if we need cooling
      if (currentTempVal > (tempSetPoint + TEMP_THRESHOLD)) {
        digitalWrite(RELAY_HEAT_PIN, LOW);   // Ensure heating is off
        digitalWrite(RELAY_COOL_PIN, HIGH);  // Turn on cooling
        currentHeatingCoolingState = 3;      // COOLING
      }
    }
  } else {
    // Ensure device is marked as inactive when off
    if (isActive == 0 && currentHeatingCoolingState != 0) {
      active->setVal(0);
    }
  }
  
  // Update the current state if it changed
  if (currentState->getVal() != currentHeatingCoolingState) {
    currentState->setVal(currentHeatingCoolingState);
  }
}

void ThermostatService::setLedIndicators() {
  // Set LED indicators based on current state
  // HeaterCooler CurrentHeaterCoolerState değerleri:
  // 0=INACTIVE, 1=IDLE, 2=HEATING, 3=COOLING
  int currentHeatingCoolingState = currentState->getVal<int>();
  
  switch (currentHeatingCoolingState) {
    case 0: // INACTIVE
      digitalWrite(HEAT_LED_PIN, LOW);
      digitalWrite(COOL_LED_PIN, LOW);
      // Switch off when both heating and cooling are off
      digitalWrite(ACTIVE_SWITCH_PIN, LOW);
      break;
    case 1: // IDLE
      digitalWrite(HEAT_LED_PIN, LOW);
      digitalWrite(COOL_LED_PIN, LOW);
      // Switch off when in idle state
      digitalWrite(ACTIVE_SWITCH_PIN, LOW);
      break;
    case 2: // HEATING
      digitalWrite(HEAT_LED_PIN, HIGH);
      digitalWrite(COOL_LED_PIN, LOW);
      // Switch on when heating is active
      digitalWrite(ACTIVE_SWITCH_PIN, HIGH);
      break;
    case 3: // COOLING
      digitalWrite(HEAT_LED_PIN, LOW);
      digitalWrite(COOL_LED_PIN, HIGH);
      // Switch on when cooling is active
      digitalWrite(ACTIVE_SWITCH_PIN, HIGH);
      break;
  }
  
  // Blink status LED if in AUTO mode
  if (currentMode == AUTO) {
    unsigned long currentMillis = millis();
    if ((currentMillis / 1000) % 2 == 0) {
      digitalWrite(STATUS_LED_PIN, HIGH);
    } else {
      digitalWrite(STATUS_LED_PIN, LOW);
    }
  } else {
    digitalWrite(STATUS_LED_PIN, HIGH); // Always on in other modes
  }
}

void ThermostatService::updateCurrentTemperature(float temperature) {
  if (currentTemp->getVal<float>() != temperature) {
    currentTemp->setVal(temperature);
  }
}

float ThermostatService::getTargetTemperature() {
  return tempSetPoint;
}

OperationMode ThermostatService::getCurrentMode() {
  return currentMode;
}

// ThermostatDevice implementation
ThermostatDevice::ThermostatDevice() {
  lastSensorReadTime = 0;
  currentTemperature = 0;
  currentHumidity = 0;
}

ThermostatDevice::~ThermostatDevice() {
  if (dht) {
    delete dht;
  }
}

void ThermostatDevice::setup() {
  // Initialize DHT sensor
  dht = new DHT(DHT_PIN, DHT_TYPE);
  dht->begin();
  
  // Setup HomeSpan
  // HomeSpan'in Category enum kullanımı için güncellendi
  homeSpan.begin(Category::Thermostats, "HomeKit-Thermostat", "ESP-Thermostat", HOMEKIT_SETUP_CODE);
  
  // Add HomeKit accessories
  new SpanAccessory();
    new Service::AccessoryInformation();
      new Characteristic::Name("HomeKit Thermostat");
      new Characteristic::Manufacturer("DIY HomeKit");
      new Characteristic::SerialNumber("HS-THERMO-001");
      new Characteristic::Model("ESP-Thermostat v1.0");
      new Characteristic::FirmwareRevision("1.0");
      new Characteristic::Identify();
      
    // Create thermostat service
    thermostat = new ThermostatService(dht);
    tempSensor = new TemperatureSensor(dht);
    humiditySensor = new HumiditySensor(dht);
  
  // Log that setup is complete
  LOG1("HomeKit Thermostat Setup Complete\n");
}

void ThermostatDevice::loop() {
  // Process HomeSpan tasks
  homeSpan.poll();
  
  // Read sensors periodically
  unsigned long currentMillis = millis();
  if (currentMillis - lastSensorReadTime >= SENSOR_READ_INTERVAL) {
    lastSensorReadTime = currentMillis;
    readSensors();
    controlRelays();
  }
}

void ThermostatDevice::readSensors() {
  // Read temperature and humidity from DHT sensor
  float newTemp = dht->readTemperature();
  float newHumidity = dht->readHumidity();
  
  // Check for valid readings
  if (!isnan(newTemp) && !isnan(newHumidity)) {
    currentTemperature = newTemp;
    currentHumidity = newHumidity;
    
    // Update the HomeKit characteristics
    thermostat->updateCurrentTemperature(currentTemperature);
    tempSensor->updateTemperature(currentTemperature);
    humiditySensor->updateHumidity(currentHumidity);
    
    // Log current values
    LOG1("Temperature: ");
    LOG1(currentTemperature);
    LOG1("°C, Humidity: ");
    LOG1(currentHumidity);
    LOG1("%, Target: ");
    LOG1(thermostat->getTargetTemperature());
    LOG1("°C, Mode: ");
    LOG1(thermostat->getCurrentMode());
    LOG1("\n");
  } else {
    LOG1("Failed to read from DHT sensor!\n");
  }
}

void ThermostatDevice::controlRelays() {
  // This is handled by the thermostat service in its loop
  thermostat->loop();
}