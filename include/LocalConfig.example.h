#pragma once

#include <Arduino.h>



constexpr const char* WIFI_SSID = "your-wifi-name";
constexpr const char* WIFI_PASSWORD = "your-wifi-password";
constexpr const char* OTA_HOSTNAME = "gate-01";

constexpr unsigned long WIFI_TIMEOUT_MS = 15000;

// Pin assingment
constexpr uint8_t SENSOR_PIN_LEFT = 34;
constexpr uint8_t SENSOR_PIN_MIDDLE = 35;
constexpr uint8_t SENSOR_PIN_RIGHT = 32;

// Sensor parameters
constexpr uint32_t SENSOR_READ_INTERVAL_MS = 100;
constexpr uint32_t SENSOR_SAMPLING = 10;