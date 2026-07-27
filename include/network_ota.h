#ifndef NETWORK_OTA_H
#define NETWORK_OTA_H

#include <Arduino.h>

// Starts non-blocking Wi-Fi and OTA initialization
void initNetwork();

// Manages background Wi-Fi reconnection and OTA uploads without blocking the main loop
void handleNetwork();

#endif // NETWORK_OTA_H