/*
  PROJECT: SMART IRRIGATION SYSTEM ADAPTIF (ESP32)
  Deskripsi: Irigasi otomatis dengan prediksi cuaca OpenWeatherMap.
  Output: Serial Monitor & Relay (Pompa).
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// --- KONFIGURASI WIFI & API ---
const char* ssid = "iPhone 11";      
const char* password = "123456899"; 
String apiKey = "09a7369a2e573c2c3d16f7bb0887ec59";    
String city = "Sleman";        
String countryCode = "ID";         

// --- PIN MAPPING ---
#define PIN_SOIL 34        // Sensor Tanah (Analog)
#define PIN_DHT 21         // Sensor DHT11 (Digital)
#define PIN_RELAY 23       // Relay Pompa (Digital Output)

#define DHTTYPE DHT11
DHT dht(PIN_DHT, DHTTYPE);

// --- VARIABEL ---
const int DRY_THRESHOLD = 40; // Batas kering (%)
bool isRainingSoon = false;   
unsigned long lastWeatherTime = 0;
unsigned long weatherInterval = 3600000; // Cek API tiap 1 jam

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, HIGH); // Default OFF (High)
  
  dht.begin();
  
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  
  checkWeather(); // Cek cuaca awal
}

void loop() {
  // 1. Cek API Berkala
  if (millis() - lastWeatherTime > weatherInterval) {
    checkWeather();
    lastWeatherTime = millis();
  }

  // 2. Baca Sensor
  int soilRaw = analogRead(PIN_SOIL);
  int soilPercent = map(soilRaw, 4095, 0, 0, 100); 
  
  float t = dht.readTemperature();

  // 3. TAMPILAN MONITOR (BUKTI UNTUK LAPORAN)
  Serial.println("--------------------------------");
  Serial.print("Suhu Udara: "); Serial.print(t); Serial.println(" C");
  Serial.print("Kelembaban Tanah: "); Serial.print(soilPercent); Serial.println(" %");
  Serial.print("Status Cuaca (API): "); 
  Serial.println(isRainingSoon ? "AKAN HUJAN" : "CERAH");

  // 4. LOGIKA UTAMA
  if (soilPercent < DRY_THRESHOLD) { // TANAH KERING
    if (isRainingSoon) {
      // LOGIKA ADAPTIF
      Serial.println("KEPUTUSAN: [TUNDA] Hemat air, akan hujan.");
      digitalWrite(PIN_RELAY, HIGH); // Pompa MATI
    } else {
      // LOGIKA NORMAL
      Serial.println("KEPUTUSAN: [SIRAM] Menyalakan Pompa.");
      digitalWrite(PIN_RELAY, LOW); // Pompa NYALA
    }
  } else { // TANAH BASAH
    Serial.println("KEPUTUSAN: [STANDBY] Tanah lembab.");
    digitalWrite(PIN_RELAY, HIGH); // Pompa MATI
  }

  delay(3000); 
}

void checkWeather() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&appid=" + apiKey;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      
      int weatherId = doc["weather"][0]["id"];
      // ID < 600 artinya Hujan/Gerimis
      isRainingSoon = (weatherId < 600); 
    }
    http.end();
  }
}