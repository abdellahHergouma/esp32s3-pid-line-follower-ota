#include "network_ota.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "ABDELLAH HE";
const char* password = "123456789000";

bool otaInitialized = false;

void initNetwork() {
  WiFi.mode(WIFI_STA);
  // DISABLE power saving mode to stop ESP32-S3 brownouts during Wi-Fi transmission
  WiFi.setSleep(false); 
  WiFi.begin(ssid, password);
  Serial.println("[NETWORK] Wi-Fi connection initiated on Core 0...");
}

void handleNetwork() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!otaInitialized) {
      Serial.println("\n[NETWORK] Wi-Fi Connected!");
      Serial.print("[NETWORK] IP Address: ");
      Serial.println(WiFi.localIP());

      ArduinoOTA.setHostname("esp32s3-pid-linefollower");
      ArduinoOTA.setPassword("admin");

      ArduinoOTA.onStart([]() {
        Serial.println("[OTA] Firmware update starting...");
      });
      ArduinoOTA.onEnd([]() {
        Serial.println("\n[OTA] Update Complete.");
      });
      ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("[OTA] Error[%u]\n", error);
      });

      ArduinoOTA.begin();
      otaInitialized = true;
    }
    
    ArduinoOTA.handle();
  } else {
    if (otaInitialized) {
      Serial.println("[NETWORK] Wi-Fi connection lost. Waiting to reconnect...");
      otaInitialized = false;
    }
  }
}