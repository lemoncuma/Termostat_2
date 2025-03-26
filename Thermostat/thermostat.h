#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#include <Arduino.h>
#include <DHT.h>
#include "HomeSpan.h"
#include "config.h"

// Forward declarations
class ThermostatService;

// Temperature Sensor Accessory
class TemperatureSensor : public Service::TemperatureSensor {
  private:
    SpanCharacteristic *currentTemp;
    DHT *dht;

  public:
    TemperatureSensor(DHT *dhtSensor);
    void updateTemperature(float temperature);
};

// Humidity Sensor Accessory
class HumiditySensor : public Service::HumiditySensor {
  private:
    SpanCharacteristic *currentHumidity;
    DHT *dht;

  public:
    HumiditySensor(DHT *dhtSensor);
    void updateHumidity(float humidity);
};

// Thermostat Accessory
// Not: HomeSpan kütüphanesindeki güncel servis adını kullanıyoruz
class ThermostatService : public Service::HeaterCooler {
  private:
    SpanCharacteristic *currentTemp;
    SpanCharacteristic *targetTemp;
    SpanCharacteristic *currentState;
    SpanCharacteristic *targetState;
    SpanCharacteristic *tempDisplayUnits;
    SpanCharacteristic *active;
    SpanCharacteristic *rotationSpeed; // Fan hızı kontrolü için
    
    DHT *dht;
    float tempSetPoint;
    OperationMode currentMode;
    
    void updateHeatingCooling();
    void setLedIndicators();

  public:
    ThermostatService(DHT *dhtSensor);
    boolean update() override;
    void loop() override;
    
    void updateCurrentTemperature(float temperature);
    float getTargetTemperature();
    OperationMode getCurrentMode();
};

// Thermostat Device - Main class for managing the thermostat
class ThermostatDevice {
  private:
    DHT *dht;
    ThermostatService *thermostat;
    TemperatureSensor *tempSensor;
    HumiditySensor *humiditySensor;
    
    unsigned long lastSensorReadTime;
    float currentTemperature;
    float currentHumidity;
    
    void readSensors();
    void controlRelays();

  public:
    ThermostatDevice();
    ~ThermostatDevice();
    
    void setup();
    void loop();
};

#endif // THERMOSTAT_H