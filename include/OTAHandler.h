#pragma once

class OTAHandler {
public:

    // Uruchomienie WiFi oraz OTA
    void begin(const char* ssid,
               const char* password,
               const char* hostname);

    // Obsługa OTA
    void update();

    // Status połączenia
    bool isConnected();

private:

    // Łączenie z WiFi
    void connectToWiFi(const char* ssid,
                       const char* password);

    // Rejestracja callbacków OTA
    void setupCallbacks();
};