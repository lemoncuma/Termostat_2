# Arduino IDE Kurulum Talimatları

Bu proje Arduino IDE ile kullanılabilir. Aşağıdaki adımları takip ederek kurulumu yapabilir ve projeyi yükleyebilirsiniz.

## 1. Gerekli Kütüphanelerin Kurulumu

Arduino IDE'de aşağıdaki kütüphaneleri yüklemeniz gerekiyor:

1. **HomeSpan Kütüphanesi**:
   - Arduino IDE'de "Araçlar > Kütüphane Yöneticisi" seçeneğini açın
   - Arama çubuğuna "HomeSpan" yazın
   - "HomeSpan by Gregg Tavares" kütüphanesini bulun ve "Yükle" düğmesine tıklayın

2. **DHT Sensör Kütüphanesi**:
   - Arduino IDE'de "Araçlar > Kütüphane Yöneticisi" seçeneğini açın
   - Arama çubuğuna "DHT" yazın
   - "DHT sensor library by Adafruit" kütüphanesini bulun ve "Yükle" düğmesine tıklayın

3. **Adafruit Unified Sensor Kütüphanesi**:
   - Arduino IDE'de "Araçlar > Kütüphane Yöneticisi" seçeneğini açın
   - Arama çubuğuna "Adafruit Unified Sensor" yazın
   - "Adafruit Unified Sensor by Adafruit" kütüphanesini bulun ve "Yükle" düğmesine tıklayın

## 2. ESP8266 veya ESP32 Kartlarının Kurulumu

### ESP8266 için:
1. Arduino IDE'de "Dosya > Tercihler" seçeneğini açın
2. "Ek Kart Yöneticisi URL'leri" alanına şu adresi ekleyin:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. "Araçlar > Kart > Kart Yöneticisi" seçeneğini açın
4. Arama kutusuna "esp8266" yazın
5. "esp8266 by ESP8266 Community" paketini yükleyin

### ESP32 için:
1. Arduino IDE'de "Dosya > Tercihler" seçeneğini açın
2. "Ek Kart Yöneticisi URL'leri" alanına şu adresi ekleyin:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. "Araçlar > Kart > Kart Yöneticisi" seçeneğini açın
4. Arama kutusuna "esp32" yazın
5. "ESP32 by Espressif Systems" paketini yükleyin

## 3. Proje Dosyalarının Hazırlanması

1. Bu projedeki aşağıdaki dosyaları yeni bir Arduino klasörüne kopyalayın:
   - `Thermostat.ino` (Ana Arduino dosyası)
   - `config.h` (Yapılandırma ayarları)
   - `thermostat.h` (Termostat sınıf tanımlamaları)
   - `thermostat.cpp` (Termostat sınıf uygulamaları)

2. Arduino IDE'de `Thermostat.ino` dosyasını açın

3. Gerekirse, WiFi ayarlarınızı `config.h` dosyasında güncelleyin:
   ```cpp
   #define WIFI_SSID "KendiWiFiAdiniz"        // WiFi SSID'nizi girin
   #define WIFI_PASSWORD "WiFiSifreniz"       // WiFi şifrenizi girin
   ```

## 4. Projeyi Yükleme

1. Arduino IDE'de "Araçlar" menüsünden kartınızı seçin:
   - ESP8266 için: "NodeMCU 1.0 (ESP-12E Module)" veya kullandığınız ESP8266 kartını seçin
   - ESP32 için: "ESP32 Dev Module" veya kullandığınız ESP32 kartını seçin

2. Doğru portu seçin ("Araçlar > Port" menüsünden)

3. "Yükle" düğmesine tıklayarak projeyi kartınıza yükleyin

4. Yükleme tamamlandıktan sonra, "Araçlar > Seri Monitör" seçeneğini açın ve baud hızını 115200 olarak ayarlayın

## 5. Sorun Giderme

- Derleme hataları alırsanız, gerekli tüm kütüphanelerin yüklendiğinden emin olun
- ESP8266 kullanıyorsanız, `ESP8266_PLATFORM` tanımlaması otomatik olarak yapılacaktır
- HomeKit eşleştirme kodu: 466-37-726 (bu kodu `config.h` dosyasında değiştirebilirsiniz)

## 6. Devre Bağlantıları

### ESP8266 (NodeMCU) Bağlantıları:
- DHT21 data pin → D4
- Heating relay pin → D1
- Cooling relay pin → D2
- Status LED pin → D5
- Heating LED pin → D6
- Cooling LED pin → D7
- Active status switch pin → D3

### ESP32 Bağlantıları:
- DHT21 data pin → GPIO4
- Heating relay pin → GPIO16
- Cooling relay pin → GPIO17
- Status LED pin → GPIO5
- Heating LED pin → GPIO18
- Cooling LED pin → GPIO19
- Active status switch pin → GPIO21