/*
 * HomeKit Compatible Thermostat using ESP8266/ESP32
 * 
 * Bu Arduino sketch, ESP8266 veya ESP32 kullanarak Apple HomeKit 
 * ile uyumlu bir termostat oluşturur.
 * 
 * Desteklenen özellikler:
 * - DHT21 sensörü ile sıcaklık ve nem ölçümü
 * - Isıtma ve soğutma kontrolü
 * - HomeKit üzerinden tam kontrol
 * - LED ile durum göstergeleri
 * - Aktif durum anahtarı
 */

#if defined(ESP8266)
  #define ESP8266_PLATFORM
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif

#include "HomeSpan.h"
#include "thermostat.h"
#include "config.h"

ThermostatDevice *thermostat = nullptr;

void setup() {
  // Seri iletişimi başlat
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting HomeKit Thermostat...");
  
  // WiFi'a bağlan
  Serial.print("Connecting to WiFi ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Termostat sistemini başlat
  thermostat = new ThermostatDevice();
  thermostat->setup();
}

void loop() {
  // Termostat döngüsünü çalıştır
  if (thermostat) {
    thermostat->loop();
  }
}