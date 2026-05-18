#pragma once

#include <Arduino.h>


// Wifi
constexpr const char* WIFI_SSID = "your-wifi-name";
constexpr const char* WIFI_PASSWORD = "your-wifi-password";
constexpr const char* OTA_HOSTNAME = "gate-01";

constexpr unsigned long WIFI_TIMEOUT_MS = 15000;

// Pin assingment
constexpr uint8_t SENSOR_PIN_LEFT = 34;
constexpr uint8_t SENSOR_PIN_MIDDLE = 35;
constexpr uint8_t SENSOR_PIN_RIGHT = 32;

constexpr uint8_t LED_PIN_INNER = 20;
constexpr uint8_t LED_PIN_OUTER = 21;

// Sensor parameters
constexpr uint32_t SENSOR_READ_INTERVAL_MS = 100;
constexpr uint32_t SENSOR_SAMPLING = 10;

constexpr float LEFT_TRIGGER_DISTANCE = 40;
constexpr float MIDDLE_TRIGGER_DISTANCE = 40;
constexpr float RIGHT_TRIGGER_DISTANCE = 40;

// Led parameters
constexpr uint32_t LED_COUNT = 213;