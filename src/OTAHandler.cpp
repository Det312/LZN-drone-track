#include "OTAHandler.h"

#include <WiFi.h>
#include <ArduinoOTA.h>

void OTAHandler::begin(const char* ssid,
                       const char* password,
                       const char* hostname,
                       uint32_t timeoutMs){
    
    timeout = timeoutMs;
    
    connectToWiFi(ssid, password);

    if (!isConnected()) {

        Serial.println("[OTA] WiFi not connected");
        return;
    }

    ArduinoOTA.setHostname(hostname);
    setupCallbacks();
    ArduinoOTA.begin();

    Serial.println("[OTA] Ready");
}

void OTAHandler::update() {

    ArduinoOTA.handle();
}

bool OTAHandler::isConnected() {

    return WiFi.status() == WL_CONNECTED;
}

void OTAHandler::connectToWiFi(const char* ssid,
                               const char* password) {

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.persistent(true);
    WiFi.setSleep(false);
    WiFi.begin(ssid, password);

    Serial.print("[WiFi] Connecting");

    unsigned long startTime = millis();

    // Timeout 10 seconds
    while (WiFi.status() != WL_CONNECTED &&
           millis() - startTime < timeout) {

        Serial.print(".");
        delay(500);
    }

    Serial.println();

    // Connection info
    if (WiFi.status() == WL_CONNECTED) {

        Serial.println("[WiFi] Connected");
        Serial.print("[WiFi] IP: ");

        Serial.println(WiFi.localIP());

    } else {

        Serial.println("[WiFi] Connection failed");
    }
}

void OTAHandler::setupCallbacks() {

    ArduinoOTA.onStart([]() {

        Serial.println("[OTA] Start");
    });

    ArduinoOTA.onEnd([]() {

        Serial.println("\n[OTA] End");
    });

    ArduinoOTA.onProgress([](unsigned int progress,
                             unsigned int total) {

        uint8_t percent = (progress * 100) / total;

        Serial.printf("[OTA] Progress: %u%%\r", percent);
    });

    ArduinoOTA.onError([](ota_error_t error) {

        Serial.printf("\n[OTA] Error[%u]: ", error);

        switch(error) {

            case OTA_AUTH_ERROR:
                Serial.println("Auth Failed");
                break;

            case OTA_BEGIN_ERROR:
                Serial.println("Begin Failed");
                break;

            case OTA_CONNECT_ERROR:
                Serial.println("Connect Failed");
                break;

            case OTA_RECEIVE_ERROR:
                Serial.println("Receive Failed");
                break;

            case OTA_END_ERROR:
                Serial.println("End Failed");
                break;

            default:
                Serial.println("Unknown Error");
                break;
        }
    });
}