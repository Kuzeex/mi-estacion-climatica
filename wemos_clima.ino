// ── Wemos D1 Mini · BMP280 + HTU2X (I2C) → Servidor Flask ──────────
// Librerías requeridas:
//   - Adafruit BMP280 Library
//   - Adafruit HTU21DF Library  (by Adafruit)
//   - ArduinoJson

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_HTU21DF.h>
#include <ArduinoJson.h>

const char* SSID     = "RICARDO";
const char* PASSWORD = "197866330";
const char* SERVER_URL = "http://192.168.1.83:5000/datos";

Adafruit_BMP280 bmp;
Adafruit_HTU21DF htu = Adafruit_HTU21DF(); // <-- Instancia del HTU2X

WiFiClient client;

void setup() {
  Serial.begin(115200);

  Serial.println("\nIniciando Sensores I2C...");

  // Inicializar BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("ERROR: No se encuentra el BMP280.");
    while (1) delay(1000);
  }

  // Inicializar HTU2X
  if (!htu.begin()) {
    Serial.println("ERROR: No se encuentra el HTU2X. Revisa el bus I2C.");
    while (1) delay(1000);
  }

  // Configuración del BMP280
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, Adafruit_BMP280::SAMPLING_X2, 
                  Adafruit_BMP280::SAMPLING_X16, Adafruit_BMP280::FILTER_X16, Adafruit_BMP280::STANDBY_MS_500);

  Serial.println("Sensores listos!");

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void loop() {
  // Lecturas del BMP280
  float temp    = bmp.readTemperature();
  float presion = bmp.readPressure() / 100.0;
  float altitud = bmp.readAltitude(1013.25);
  
  // Lectura exclusiva de Humedad desde el HTU2X
  float humedad = htu.readHumidity(); 
  
  int wifi_signal = WiFi.RSSI();

  // Si la lectura falla, le asignamos 0 para evitar enviar un "NaN" corrupto
  if (isnan(humedad)) {
    humedad = 0.0;
  }

  Serial.printf("Temp: %.1f°C  Presión: %.1f hPa  Humedad: %.1f%%  WiFi: %d dBm\n",
                temp, presion, humedad, wifi_signal);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(client, SERVER_URL);
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<128> doc;
    doc["temp"]    = round(temp * 10) / 10.0;
    doc["presion"] = round(presion * 10) / 10.0;
    doc["altitud"] = round(altitud);
    doc["wifi"]    = wifi_signal;
    doc["humedad"] = round(humedad * 10) / 10.0; // <-- Mandamos la humedad a Flask

    String body;
    serializeJson(doc, body);
    int httpCode = http.POST(body);
    http.end();
  }
  
  delay(10000);
}