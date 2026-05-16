#pragma once

class OTAHandler {
public:

    //Initializes WiFi and starts OTA service
    void begin(const char* ssid,
               const char* password,
               const char* hostname,
               const int timeoutMs);

    // Must be called frequently from loop() to keep OTA responsive.
    // Blocking delays can cause issues with OTA service
    void update();

    //Returns wifi connection status
    bool isConnected();

private: 
    int timeout; //Wifi connection timeout period

    //Connect to a wifi network for ota service
    void connectToWiFi(const char* ssid,
                       const char* password);

    //Registers OTA event callbacks for serial logging and debug
    void setupCallbacks();
};