#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "ComProtocol.h"

class BrokerCom {
public:
    bool begin(const uint8_t brokerMac[6], uint8_t gateId);

    bool sendTrigger();

    bool hasCommand() const;
    BrokerCommandMessage readCommand();

private:
    static BrokerCom* s_instance;

    uint8_t m_brokerMac[6];
    uint8_t m_gateId = 0;

    uint16_t m_eventCounter = 0;

    volatile bool m_hasCommand = false;
    BrokerCommandMessage m_lastCommand = {};

    static void onDataReceived(const uint8_t* mac,
                               const uint8_t* data,
                               int len);

    void handleReceivedData(const uint8_t* data, int len);
};