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
  WiFi.begin(ssid, password);
  Serial.println("Wi-Fi connection initiated in background...");
}

void handleNetwork() {
  // Check if Wi-Fi connected, then initialize OTA exactly once
  if (WiFi.status() == WL_CONNECTED) {
    if (!otaInitialized) {
      Serial.println("\nWi-Fi Connected successfully!");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());

      ArduinoOTA.setHostname("esp32s3-n16r8-device");
      ArduinoOTA.setPassword("admin");
      ArduinoOTA.begin();
      
      otaInitialized = true;
    }
    
    // Process OTA requests if connected
    ArduinoOTA.handle();
  } else {
    // If connection drops, reset initialization flag to allow re-init when network returns
    if (otaInitialized) {
      Serial.println("Wi-Fi disconnected. Waiting for reconnection...");
      otaInitialized = false;
    }
  }
}