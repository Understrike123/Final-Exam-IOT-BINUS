#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Node IOT Rack Server 2

// Konfigurasi WiFi 
const char* ssid = "Understriker";
const char* password = "tdfn1103";

// Konfigurasi MQTT ThingsBoard 
const char* mqttServer = "demo.thingsboard.io";
const int mqttPort = 1883;
const char* accessToken = "p31cioydglar0jyf7bvj";  // Token dari masin-masing device ke ThingsBoard

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// Konfigurasi DHT22 
#define DHTPIN 27  // Pin DHT 22 pada GPIO 27
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastSend = 0;

void connectToWiFi() {
  Serial.print("Menghubungkan ke WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTerhubung ke WiFi!");
}

void connectToMQTT() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke ThingsBoard MQTT...");
    
    // ESP 32 connecting ke Thingsboard melalui link dengan data device di Thingsboard
    if (client.connect("ESP32Client", accessToken, NULL)) {
      Serial.println("Terhubung ke ThingsBoard!");
    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 5 detik");
      delay(5000);
    }
  }
}

void setup() {
  // Program berjalan di serial monitor 115200
  Serial.begin(115200);
  dht.begin();
  connectToWiFi();
  client.setServer(mqttServer, mqttPort);
  connectToMQTT();
}

void loop() {
  if (!client.connected()) {
    connectToMQTT();
  }
  client.loop();

  // Mengirim status data temperature dan humidity ke Thingsboard setiap 5 detik sekali
  if ((millis() - lastSend) > 5000 && WiFi.status() == WL_CONNECTED) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Gagal membaca sensor DHT22!");
      return;
    }

    // Buat payload JSON ke Thingsboard
    String payload = "{";
    payload += "\"temperature\":";
    payload += temperature;
    payload += ",\"humidity\":";
    payload += humidity;
    payload += "}";

    client.publish("v1/devices/me/telemetry", payload.c_str());

    // Data status sudah terkirim ke Thingsboard
    Serial.print("Data terkirim: ");
    Serial.println(payload);
    Serial.print("Waktu millis saat kirim: ");
    Serial.println(millis());

    lastSend = millis();
  }
}
